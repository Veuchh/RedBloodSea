// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveSpawnManager.h"

#include "K2Node_GetSubsystem.h"


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
	if(SpawnQueue.IsEmpty() || AliveDwellers.Num()>=MaxAliveDweller || !bIsActive)
		return;
	for(int i = 0; i < SpawnPerTick; i++)
	{
		if(SpawnQueue.IsEmpty() || AliveDwellers.Num()>=MaxAliveDweller)
			break;
		
		FDwellerProfile Type;
		SpawnQueue.Dequeue(Type);
		SpawnDweller(Type);
	}

	if(SpawnQueue.IsEmpty())
	{
		if(CurrentWave < WavesData->Waves.Num()-1)
			QueueWave(++CurrentWave);
		else if(AliveDwellers.IsEmpty())
		{
			//TODO End level here
		}
	}
}

void AWaveSpawnManager::SpawnStart()
{
	bIsActive = true;
	QueueWave(0);
}

void AWaveSpawnManager::SpawnStop()
{
	bIsActive = false;
	SpawnReset();
}

void AWaveSpawnManager::SpawnPause()
{
	bIsActive = false;
}

void AWaveSpawnManager::SpawnResume()
{
	bIsActive = true;
}

void AWaveSpawnManager::SpawnReset()
{
	SpawnQueue.Empty();
	CurrentWave = 0;
	for (auto Dweller : AliveDwellers)
	{
		Dweller->Destroy();
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
}

void AWaveSpawnManager::OnDwellerDeath(AActor* DwellerActor)
{
	ADweller* Dweller = Cast<ADweller>(DwellerActor);
	if(Dweller)
	{
		AliveDwellers.Remove(Dweller);
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

