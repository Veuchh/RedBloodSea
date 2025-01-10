// Fill out your copyright notice in the Description page of Project Settings.


#include "CountLevelInstanceSubsystem.h"

void UCountLevelInstanceSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	OnKill.AddUniqueDynamic(this,&UCountLevelInstanceSubsystem::AddKill);
	OnLinked.AddUniqueDynamic(this,&UCountLevelInstanceSubsystem::AddLinked);
}

void UCountLevelInstanceSubsystem::Deinitialize()
{
	Super::Deinitialize();
	OnKill.RemoveDynamic(this,&UCountLevelInstanceSubsystem::AddKill);
	OnLinked.RemoveDynamic(this,&UCountLevelInstanceSubsystem::AddLinked);
}

float UCountLevelInstanceSubsystem::GetKillPercent()
{
	float killPercent = static_cast<float>(killCount) / static_cast<float>(killCount+possessCount);
	return killPercent;
}

void UCountLevelInstanceSubsystem::AddKill()
{
	killCount++;
	OnCountChange.Broadcast(GetKillPercent());
}

void UCountLevelInstanceSubsystem::AddLinked(int count)
{
	possessCount += count;
	OnCountChange.Broadcast(GetKillPercent());
}

void UCountLevelInstanceSubsystem::ResetAll()
{
	possessCount = 0;
	killCount = 0;
}

