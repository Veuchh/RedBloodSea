// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveSpawnManager.h"


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
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("WaveSpawnManagerBegin"));
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
		
			FDwellerProfile Type;
			SpawnQueue.Dequeue(Type);
			SpawnDweller(Type);
		}
	}

	if(SpawnQueue.IsEmpty())
	{
		if(AliveDwellers.IsEmpty() && bIsActive)
		{
			if(CurrentWave < WavesData->Waves.Num()-1)
			{
				QueueWave(++CurrentWave);
				return;
			}
			//TODO End level here
			bIsActive = false;
			OnLevelEnd.Broadcast();
			if(GEngine)
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Level End: All enemies are dead or linked"));
		}
	}
}

void AWaveSpawnManager::SpawnStart()
{
	bIsActive = true;
	SetActorTickEnabled(true);
	QueueWave(0);
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
	for (auto Dweller : AliveDwellers)
	{
		if(IsValid(Dweller))
		{
			Dweller->Destroy();
		}
	}
	
	AliveDwellers.Empty();
}

void AWaveSpawnManager::QueueWave(int WaveNumber)
{
	for (auto Wave : WavesData->Waves)
	{
		for (auto Profile : Wave.DwellerProfiles)
		{
			SpawnQueue.Enqueue(Profile);
		}
	}
}

void AWaveSpawnManager::SpawnDweller(FDwellerProfile Type)
{
	FTransform Transform = GetActorTransform();
	//TODO use EQS to get random valid position on the navmesh instead
	if(Spawners.Num() > 0)
	{
		int randomInt = FMath::RandRange(0, Spawners.Num() - 1);
		Transform = Spawners[randomInt]->GetActorTransform();
	}
	ADweller* newDweller = GetWorld()->SpawnActorDeferred<ADweller>(WavesData->DwellerBP,Transform);
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
		
	}
	newDweller->FinishSpawning(Transform);
	//if(GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Spawning a Dweller"));
}

void AWaveSpawnManager::OnDwellerDeath(AActor* DwellerActor)
{
	ADweller* Dweller = Cast<ADweller>(DwellerActor);
	if(Dweller)
	{
		AliveDwellers.Remove(Dweller);
	}
	if(AliveDwellers.Num() == 0 && CurrentWave >= WavesData->Waves.Num())
	{
		OnLevelEnd.Broadcast();
	}
}

#if WITH_EDITOR
void AWaveSpawnManager::CreateSpawner()
{
	AActor* newActor = GetWorld()->SpawnActor(WavesData->SpawnerBP);
	newActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Spawners.Add(newActor);


	//This should work but doesn't fml
	//FEditorViewportClient* client = (FEditorViewportClient*)GEditor->GetActiveViewport()->GetClient();
	// if(client != NULL)
	// {
	// 	FVector CamForward = client->GetViewRotation().Vector();
	// 	FVector CamLocation = client->GetViewLocation();	
	// 	FHitResult Hit;
	// 	if(GetWorld()->LineTraceSingleByChannel(Hit,CamLocation,CamLocation + CamForward*10000,ECC_WorldStatic,FCollisionQueryParams(),FCollisionResponseParams()))
	// 	{
	// 		newActor->SetActorLocation(Hit.Location);
	// 		return;
	// 	}
	// }
	newActor->SetActorLocation(this->GetActorLocation());
}

void AWaveSpawnManager::ClearSpawners()
{
	for (auto Spawner : Spawners)
	{
		if(IsValid(Spawner))
		{
			Spawner->Destroy();
		}
		
	}
	Spawners.Empty();
}
#endif

