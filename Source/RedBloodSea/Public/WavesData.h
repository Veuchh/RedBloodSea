// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeakpointData.h"
#include "Engine/DataAsset.h"
#include "WavesData.generated.h"


USTRUCT(BlueprintType)
struct FDwellerType
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category="Weakpoints Handler",meta=(Bitmask,BitmaskEnum = EWeakpointType))
	uint8 TypeFilter;
	UPROPERTY(EditAnywhere, Category="Weakpoints Handler",meta=(ArraySizeEnum="EWeakpointSize"))
	int SizeNumber[static_cast<int>(EWeakpointSize::NUM)];
}; UMETA(DisplayName="Wave")

USTRUCT(BlueprintType)
struct FDwellerWave
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TMap<FDwellerType,int> DwellerCount;
}; UMETA(DisplayName="Wave")

/**
 * 
 */
UCLASS()
class REDBLOODSEA_API UWavesData : public UDataAsset
{
	GENERATED_BODY()
	
};
