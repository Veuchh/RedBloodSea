// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WavesData.h"
#include "GameFramework/Actor.h"
#include "WaveSpawnManager.generated.h"

UCLASS()
class REDBLOODSEA_API AWaveSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveSpawnManager();

private:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UWavesData> WavesData;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void StartSpawningWaves();
	void SpawnWave(int WaveNumber);



	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelStart);
	FLevelStart OnLevelStart;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelEnd);
	FLevelEnd OnLevelEnd;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaveStart);
	FWaveStart OnWaveStart;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaveEnd);
	FWaveEnd OnWaveEnd;
};
