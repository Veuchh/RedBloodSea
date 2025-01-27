// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveSpawnManager.h"

#include "PlayerData.h"
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
	
	CurrentWave = 0;
	WavePrepare();
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
				return;
		
			TTuple<TObjectPtr<AActor>,FDwellerProfile> Info;
			SpawnQueue.Dequeue(Info);
			if(IsValid(Info.Key))
			{
				SpawnDweller(Info.Key->GetTransform(),Info.Value);
			}
			else
			{
				SpawnDweller(this->GetTransform(),Info.Value);
			}
		}
	}

	if(SpawnQueue.IsEmpty())
	{
		if(AliveDwellers.IsEmpty() && bIsActive)
		{
			bIsActive = false;
			WaveEnd();
		}
	}
}

void AWaveSpawnManager::WavePrepare()
{
	if(Waves.Num() > CurrentWave && IsValid(Waves[CurrentWave].BeginWaveTriggerZone))
	{
		Waves[CurrentWave].BeginWaveTriggerZone->OnActorBeginOverlap.AddUniqueDynamic(this,&AWaveSpawnManager::OnBeginTriggerOverlap);
	}
}


void AWaveSpawnManager::WaveStart()
{
	bIsActive = true;
	SetActorTickEnabled(true);
	ClearAliveDwellers(false);
	QueueWave(CurrentWave);
	if(Waves[CurrentWave].bTimeLimit)
	{
		if(Waves[CurrentWave].Type != EWaveType::SURVIVE)
		{
			GetWorld()->GetTimerManager().SetTimer(CurrentWaveTimer, this, &AWaveSpawnManager::WaveFail,
											  Waves[CurrentWave].Duration, false);
		} else
		{
			GetWorld()->GetTimerManager().SetTimer(CurrentWaveTimer, this, &AWaveSpawnManager::WaveEnd,
										   Waves[CurrentWave].Duration, false);
		}
	}

	if(Waves[CurrentWave].Type ==  EWaveType::CHECKPOINT && IsValid(Waves[CurrentWave].CheckpointTriggerZone))
	{
		Waves[CurrentWave].CheckpointTriggerZone->OnActorBeginOverlap.AddUniqueDynamic(this,&AWaveSpawnManager::OnCheckpointBeginOverlap);
	}
}


void AWaveSpawnManager::WaveEnd()
{
 	bIsActive = false;
	SpawnQueue.Empty();
	SetActorTickEnabled(true);
	ClearAliveDwellers(false);
	if(Waves[CurrentWave].bTimeLimit)
	{
		GetWorld()->GetTimerManager().ClearTimer(CurrentWaveTimer);
	}
	CurrentWave++;
	if(Waves.Num() > CurrentWave)
	{
		WavePrepare();
		OnWaveSuccess.Broadcast();
	} else
	{
		OnLevelEnd.Broadcast();
	}
}

void AWaveSpawnManager::WaveFail()
{
	WaveReset();
	OnWaveFail.Broadcast();
}

void AWaveSpawnManager::WaveReset()
{
	bIsActive = false;
	SetActorTickEnabled(false);
	SpawnQueue.Empty();
	GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
	CurrentWave = 0;
	ClearAliveDwellers(true);
	WavePrepare();
}

void AWaveSpawnManager::QueueWave(int WaveNumber)
{

	for (auto Profile : Waves[CurrentWave].DwellerProfiles)
	{
		SpawnQueue.Enqueue(Profile);
	}
}

void AWaveSpawnManager::SpawnDweller(FTransform Transform, FDwellerProfile Type)
{
	ADweller* newDweller = GetWorld()->SpawnActorDeferred<ADweller>(DwellerBP,Transform);
	if(newDweller)
	{
		AliveDwellers.Add(newDweller);
		UWeakpointsManager* WeakpointsManager = newDweller->GetWeakpointManager();
		WeakpointsManager->OnDeath.AddUniqueDynamic(this,&AWaveSpawnManager::OnDwellerDeath);

		for(int i = 0; i <= static_cast<int>(EWeakpointSize::NUM); i++)
		{
			WeakpointsManager->SizeNumber[i] = Type.SizeNumber[i];
		}
		WeakpointsManager->TypeFilter = Type.TypeFilter;
		
		if(Type.bIsAntagonist)
			newDweller->Tags.Add("Antagonist");

		newDweller->InitState = Type.State;

		newDweller->FinishSpawning(Transform);

		UPossessTarget* PossessTarget = newDweller->GetComponentByClass<UPossessTarget>();
		PossessTarget->OnLinked.AddUniqueDynamic(this,&AWaveSpawnManager::OnDwellerLinked);
	}
}

void AWaveSpawnManager::ClearAliveDwellers(bool RemovePlayerDweller)
{
	ADweller* PlayerDweller = Cast<ADweller>(PlayerData::CurrentPossessTarget->GetOwner());
	for (auto Dweller : AliveDwellers)
	{
		if(IsValid(Dweller) && (RemovePlayerDweller || Dweller != PlayerDweller))
		{
			RemoveDweller(Dweller,false);
			Dweller->Destroy();
		}
	}

	if(!RemovePlayerDweller)
	{
		AliveDwellers.RemoveAll([PlayerDweller](ADweller* Dweller) {
		   return Dweller != PlayerDweller;
	   });
	} else
	{
		AliveDwellers.Empty();
	}
}

bool AWaveSpawnManager::CheckObjectives()
{
	if(Waves[CurrentWave].Type == EWaveType::CLEAR_ALL)
	{
		return AliveDwellers.Num() == 0;
	}
	return false;
}

void AWaveSpawnManager::OnDwellerDeath(AActor* DwellerActor)
{
	ADweller* Dweller = Cast<ADweller>(DwellerActor);
	if(Dweller)
	{
		AliveDwellers.Remove(Dweller);
		Waves[CurrentWave].DwellerKilled++;
	}
	if((Waves[CurrentWave].Type == EWaveType::CLEAR_SOME && Waves[CurrentWave].DwellerLinked + Waves[CurrentWave].DwellerKilled >= Waves[CurrentWave].DwellerToRemove)
		||	AliveDwellers.Num())
	{
		WaveEnd();
		OnWaveSuccess.Broadcast();
	}
}

void AWaveSpawnManager::OnDwellerLinked(AActor* Actor)
{
	ADweller* Dweller = Cast<ADweller>(Actor);
	if(Dweller)
	{
		AliveDwellers.Remove(Dweller);
		Waves[CurrentWave].DwellerLinked++;
	}
	if((Waves[CurrentWave].Type == EWaveType::CLEAR_SOME && Waves[CurrentWave].DwellerLinked + Waves[CurrentWave].DwellerKilled >= Waves[CurrentWave].DwellerToRemove)
		|| (AliveDwellers.Num() <= 1))
	{
		WaveEnd();
		OnWaveSuccess.Broadcast();
	}

}

void AWaveSpawnManager::AddDweller(ADweller* Dweller)
{
	AliveDwellers.Add(Dweller);
	UWeakpointsManager* WeakpointsManager = Dweller->GetWeakpointManager();
	UPossessTarget* PossessTarget = Dweller->GetComponentByClass<UPossessTarget>();
	WeakpointsManager->OnDeath.AddUniqueDynamic(this,&AWaveSpawnManager::OnDwellerDeath);
	PossessTarget->OnLinked.AddUniqueDynamic(this,&AWaveSpawnManager::OnDwellerLinked);
}

void AWaveSpawnManager::RemoveDweller(ADweller* Dweller,bool bRemoveFromArray)
{
	if(bRemoveFromArray)
		AliveDwellers.Remove(Dweller);
	UWeakpointsManager* WeakpointsManager = Dweller->GetWeakpointManager();
	UPossessTarget* PossessTarget = Dweller->GetComponentByClass<UPossessTarget>();
	WeakpointsManager->OnDeath.RemoveDynamic(this,&AWaveSpawnManager::OnDwellerDeath);
	PossessTarget->OnLinked.RemoveDynamic(this,&AWaveSpawnManager::OnDwellerLinked);
}

void AWaveSpawnManager::OnBeginTriggerOverlap(AActor* OverlapedActor, AActor* OtherActor)
{
	Waves[CurrentWave].BeginWaveTriggerZone->OnActorBeginOverlap.RemoveDynamic(this,&AWaveSpawnManager::OnBeginTriggerOverlap);
	WaveStart();
}

void AWaveSpawnManager::OnCheckpointBeginOverlap(AActor* OverlapedActor, AActor* OtherActor)
{
	Waves[CurrentWave].CheckpointTriggerZone->OnActorBeginOverlap.RemoveDynamic(this,&AWaveSpawnManager::OnCheckpointBeginOverlap);
	WaveEnd();
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

void AWaveSpawnManager::PostEditChangeProperty(struct FPropertyChangedEvent& e)
{
	
	Super::PostEditChangeProperty(e);
	FName PropertyName = (e.Property != NULL) ? e.Property->GetFName() : NAME_None;
	// if (PropertyName == GET_MEMBER_NAME_CHECKED(TArray<FWave>, Waves))
	// {
	// 	WeakpointsSockets.Empty();
	// 	UpdateSockets();
	// }
}
#endif

