// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dweller.h"
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
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess = "true"),Category="DwellerSpawners")
	TObjectPtr<UWavesData> WavesData;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess = "true"),EditFixedSize,Category="DwellerSpawners")
	TArray<AActor*> Spawners;
	//UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess = "true"),EditFixedSize,Category="DwellerSpawners")
	TQueue<FDwellerProfile,EQueueMode::Spsc> SpawnQueue;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess = "true"),EditFixedSize,Category="DwellerSpawners")
	TArray<ADweller*> AliveDwellers;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess = "true",ClampMin=1),EditFixedSize,Category="DwellerSpawners")
	int SpawnPerTick = 1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess = "true",ClampMin=1),EditFixedSize,Category="DwellerSpawners")
	int MaxAliveDweller = 10;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess = "true"),EditFixedSize,Category="DwellerSpawners")
	int CurrentWave;

	bool bIsActive = false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintCallable)
	void SpawnStart();
	UFUNCTION(BlueprintCallable)
	void SpawnStop();
	UFUNCTION(BlueprintCallable)
	void SpawnPause();
	UFUNCTION(BlueprintCallable)
	void SpawnResume();
	UFUNCTION(BlueprintCallable)
	void SpawnReset();
	void QueueWave(int WaveNumber);
	void SpawnDweller(FDwellerProfile Type);

	UFUNCTION()
	void OnDwellerDeath(AActor* DwellerActor);

#if WITH_EDITOR
	//Editor functions
private:
	UFUNCTION(CallInEditor,Category="DwellerSpawners")
	void CreateSpawner();
	UFUNCTION(CallInEditor,Category="DwellerSpawners")
	void ClearSpawners();
#endif

	
public:
	//EVENTS
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelStart);
	FLevelStart OnLevelStart;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelEnd);
	FLevelEnd OnLevelEnd;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaveStart);
	FWaveStart OnWaveStart;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaveEnd);
	FWaveEnd OnWaveEnd;
};
