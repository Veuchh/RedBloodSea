// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveSpawnManager.h"

#include "Subsystems/UnrealEditorSubsystem.h"


// Sets default values
AWaveSpawnManager::AWaveSpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWaveSpawnManager::BeginPlay()
{
	Super::BeginPlay();
	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("WaveSpawnManagerBegin"));
}

// Called every frame
void AWaveSpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!SpawnQueue.IsEmpty())
	{
		for(int i = 0; i < SpawnPerTick; i++)
		{
			if(SpawnQueue.IsEmpty() || AliveDwellers.Num()>=MaxAliveDweller)
				break;
		
			TTuple<TObjectPtr<AActor>,FDwellerProfile> Info;
			SpawnQueue.Dequeue(Info);
			SpawnDweller(Info.Key->GetTransform(),Info.Value);
		}
	}

	if(SpawnQueue.IsEmpty())
	{
		if(AliveDwellers.IsEmpty() && bIsActive)
		{
			if(CurrentWave < Waves.Num()-1)
			{
				QueueWave(++CurrentWave);
				return;
			}
			//TODO End level here
			bIsActive = false;
			OnLevelEnd.Broadcast();
			if(GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Level End: All enemies are dead or linked"));
		}
	}
}

void AWaveSpawnManager::SpawnStart()
{
	bIsActive = true;
	SetActorTickEnabled(true);
	QueueWave(0);
	if(Waves[CurrentWave].bHasTimer)
	{
		
	}
	
 	if(GEngine)
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, TEXT("Start Spawning Dwellers"));
}

void AWaveSpawnManager::SpawnStop()
{
	bIsActive = false;
	SetActorTickEnabled(false);
	SpawnReset();
}

void AWaveSpawnManager::SpawnPause()
{
	bIsActive = false;
	SetActorTickEnabled(false);
}

void AWaveSpawnManager::SpawnResume()
{
	bIsActive = true;
	SetActorTickEnabled(true);
}

void AWaveSpawnManager::SpawnReset()
{
	bIsActive = false;
	SetActorTickEnabled(false);
	SpawnQueue.Empty();
	CurrentWave = 0;
	ClearAliveDwellers();
}

void AWaveSpawnManager::StartWave(int WaveNumber)
{
	QueueWave(WaveNumber);
}

void AWaveSpawnManager::EndCurrentWave()
{
	SpawnQueue.Empty();
}

void AWaveSpawnManager::QueueWave(int WaveNumber)
{
	for (auto Wave : Waves)
	{
		for (auto Profile : Wave.DwellerProfiles)
		{
			SpawnQueue.Enqueue(Profile);
		}
	}
}

void AWaveSpawnManager::SpawnDweller(FTransform Transform, FDwellerProfile Type)
{
	ADweller* newDweller = GetWorld()->SpawnActorDeferred<ADweller>(DwellerBP,Transform);
	if(newDweller)
	{
		AliveDwellers.Add(newDweller);
		UWeakpointsManager* weakpointsManager = newDweller->GetWeakpointManager();
		weakpointsManager->OnDeath.AddUniqueDynamic(this,&AWaveSpawnManager::OnDwellerDeath);
		for(int i = 0; i <= static_cast<int>(EWeakpointSize::NUM); i++)
		{
			weakpointsManager->SizeNumber[i] = Type.SizeNumber[i];
		}
		weakpointsManager->TypeFilter = Type.TypeFilter;
		
		if(Type.bIsAntagonist)
			newDweller->Tags.Add("Antagonist");

		newDweller->FinishSpawning(Transform);
	}
	//if(GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Spawning a Dweller"));
}

void AWaveSpawnManager::ClearAliveDwellers()
{
	for (auto Dweller : AliveDwellers)
	{
		if(IsValid(Dweller))
		{
			Dweller->Destroy();
		}
	}
	
	AliveDwellers.Empty();
}

void AWaveSpawnManager::OnDwellerDeath(AActor* DwellerActor)
{
	ADweller* Dweller = Cast<ADweller>(DwellerActor);
	if(Dweller)
	{
		AliveDwellers.Remove(Dweller);
	}
	if(AliveDwellers.Num() <= 1 && CurrentWave >= Waves.Num())
	{
		OnLevelEnd.Broadcast();
	}
}

void AWaveSpawnManager::AddDweller(ADweller* Dweller)
{
	AliveDwellers.Add(Dweller);
	UWeakpointsManager* weakpointsManager = Dweller->GetWeakpointManager();
	weakpointsManager->OnDeath.AddUniqueDynamic(this,&AWaveSpawnManager::OnDwellerDeath);
}

void AWaveSpawnManager::RemoveDweller(ADweller* Dweller)
{
	AliveDwellers.Remove(Dweller);
	UWeakpointsManager* weakpointsManager = Dweller->GetWeakpointManager();
	weakpointsManager->OnDeath.RemoveDynamic(this,&AWaveSpawnManager::OnDwellerDeath);
}

#if WITH_EDITOR
void AWaveSpawnManager::CreateSpawner()
{
	AActor* newActor = GetWorld()->SpawnActor(SpawnerBP);
	newActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	while(Waves.Num()-1 < CurrentWave)
	{
		Waves.Add(FWave());
	}
	Waves[CurrentWave].DwellerProfiles.Add(newActor);
	
	//This should work but doesn't fml
	// if(GEditor)
	// {
	// 	UUnrealEditorSubsystem* Subsys = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>();
	// 	if(Subsys != NULL)
	// 	{
	// 		FVector Pos;
	// 		FRotator Rot;
	// 		Subsys->GetLevelViewportCameraInfo(Pos,Rot);
	// 		FHitResult Hit;
	//  	
	// 		if(GetWorld()->LineTraceSingleByChannel(Hit,Pos,Pos + Rot.Vector() * 10000,ECC_WorldStatic,FCollisionQueryParams(),FCollisionResponseParams()))
	// 		{
	// 			newActor->SetActorLocation(Hit.Location);
	// 			return;
	// 		}
	// 	}
	// }
	
	newActor->SetActorLocation(this->GetActorLocation());
}

void AWaveSpawnManager::ClearSpawners()
{
	for (auto Wave : Waves)
	{
		for (auto Spawner : Wave.DwellerProfiles)
		{
			if(IsValid(Spawner.Key))
			{
				Spawner.Key->Destroy();
			}
		}
		Wave.DwellerProfiles.Empty();
	}
}
#endif

