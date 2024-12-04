// Fill out your copyright notice in the Description page of Project Settings.


#include "CountLevelInstanceSubsystem.h"

void UCountLevelInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	OnKill.AddUniqueDynamic(this,&UCountLevelInstanceSubsystem::AddKill);
	OnPossess.AddUniqueDynamic(this,&UCountLevelInstanceSubsystem::AddPossess);
}

void UCountLevelInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();
	OnKill.RemoveDynamic(this,&UCountLevelInstanceSubsystem::AddKill);
	OnPossess.RemoveDynamic(this,&UCountLevelInstanceSubsystem::AddPossess);
}

void UCountLevelInstanceSubsystem::GetKillPercent()
{
	float killPercent = static_cast<float>(killCount) / static_cast<float>(killCount+possessCount);
	OnCountChange.Broadcast(killPercent);
}

void UCountLevelInstanceSubsystem::AddKill()
{
	killCount++;
}

void UCountLevelInstanceSubsystem::AddPossess()
{
	possessCount++;
}

