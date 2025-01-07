// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeakpointData.h"
#include "Engine/DataAsset.h"
#include "WavesData.generated.h"


USTRUCT(BlueprintType)
struct FDwellerProfile
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,meta=(Bitmask,BitmaskEnum = EWeakpointType))
	uint8 TypeFilter = static_cast<int>(EWeakpointType::HEAD) | static_cast<int>(EWeakpointType::ARMS) | static_cast<int>(EWeakpointType::LEGS) | static_cast<int>(EWeakpointType::TORSO);
	UPROPERTY(EditAnywhere,meta=(ArraySizeEnum="EWeakpointSize"))
	int SizeNumber[static_cast<int>(EWeakpointSize::NUM)];
	UPROPERTY(EditAnywhere)
	bool bIsAntagonist = false;
}; UMETA(DisplayName="Wave")

USTRUCT(BlueprintType)
struct FDwellerWave
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TArray<FDwellerProfile> DwellerProfiles;
}; UMETA(DisplayName="Wave")

/**
 * 
 */
UCLASS()
class REDBLOODSEA_API UWavesData : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	const TObjectPtr<UClass> SpawnerBP;
	UPROPERTY(EditAnywhere)
	const TObjectPtr<UClass> DwellerBP;
	UPROPERTY(EditAnywhere)
	TArray<FDwellerWave> Waves;
	
};
