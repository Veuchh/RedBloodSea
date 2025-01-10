// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LinkSplineVFX.h"
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

	UPROPERTY()
	AActor* playerActor;
	UPROPERTY()
	TArray<UPossessTarget*> dwellersInLink;
	
	UPROPERTY()
	TSubclassOf<ALinkSplineVFX> splineLinkVFXReference;

	UPROPERTY()
	TArray<ALinkSplineVFX*> instantiatedLinksVFX;
	
public:
	UFUNCTION(BlueprintCallable)
	void ResetLink();
	void AddDwellerToLink(UPossessTarget* dweller);
	void OnDwellerKilled(UPossessTarget* killedDweller);
	void SetupSubsystem(AActor* playerActor, TSubclassOf<ALinkSplineVFX> splineLinkVFXReference);
};
