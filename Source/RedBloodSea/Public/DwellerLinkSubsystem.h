// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PossessTarget.h"
#include "DwellerLinkSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class REDBLOODSEA_API UDwellerLinkSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	TArray<UPossessTarget*> dwellersInLink;


public:
	void ResetLink();
	void UpdateLinkGFX(FVector currentPlayerPosition);
	void AddDwellerToLink(UPossessTarget* dweller);
	void OnDwellerKilled(UPossessTarget* dweller);
};
