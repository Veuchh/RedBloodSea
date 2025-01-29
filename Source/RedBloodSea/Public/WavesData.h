// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeakpointData.h"
#include "Engine/DataAsset.h"
#include "WavesData.generated.h"



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
	//UPROPERTY(EditAnywhere)
	//TArray<FWave> Waves;
	
};
