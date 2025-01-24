// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Dweller.h"
#include "GameFramework/Actor.h"
#include "WaveSpawnManager.generated.h"


UENUM()
enum class EWaveType
{
	SURVIVE,
	CLEAR_ALL,
	RUN
};

USTRUCT(BlueprintType)
struct FDwellerProfile
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,meta=(Bitmask,BitmaskEnum = EWeakpointType))
	uint8 TypeFilter = static_cast<int>(EWeakpointType::HEAD) | static_cast<int>(EWeakpointType::ARMS) | static_cast<int>(EWeakpointType::LEGS) | static_cast<int>(EWeakpointType::TORSO);
	UPROPERTY(EditAnywhere,meta=(ArraySizeEnum="EWeakpointSize"))
	int SizeNumber[static_cast<int>(EWeakpointSize::NUM)] = {1};
	UPROPERTY(EditAnywhere)
	bool bIsAntagonist = false;
}; UMETA(DisplayName="Wave")

USTRUCT(BlueprintType)
struct FWave
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	bool bHasTimer;
	UPROPERTY(EditAnywhere)
	FTimespan Duration;
	UPROPERTY(EditAnywhere)
	TMap<TObjectPtr<AActor>,FDwellerProfile> DwellerProfiles;

private:
	FDateTime StartTime;
	FDateTime EndTime;
	
}; UMETA(DisplayName="Wave")


UCLASS()
class REDBLOODSEA_API AWaveSpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaveSpawnManager();

private:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess = "true"),EditFixedSize,Category="WavesParams")
	TArray<ADweller*> AliveDwellers;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess = "true",ClampMin=1),EditFixedSize,Category="WavesParams")
	int SpawnPerTick = 1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess = "true",ClampMin=1),EditFixedSize,Category="WavesParams")
	int MaxAliveDweller = 10;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess = "true",ClampMin=1),EditFixedSize,Category="WavesParams")
	const TObjectPtr<UClass> SpawnerBP;
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess = "true",ClampMin=1),EditFixedSize,Category="WavesParams")
	const TObjectPtr<UClass> DwellerBP;

	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,meta=(AllowPrivateAccess = "true"),Category="Waves")
	TArray<FWave> Waves;
	UPROPERTY(EditAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess = "true"),EditFixedSize,Category="Waves")
	int CurrentWave;

	bool bIsActive = false;
	
protected:
	TQueue<TTuple<TObjectPtr<AActor>,FDwellerProfile>,EQueueMode::Spsc> SpawnQueue;
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
	UFUNCTION()
	void StartWave(int WaveNumber);
	void EndCurrentWave();
	void QueueWave(int WaveNumber);
	void SpawnDweller(FTransform Transform, FDwellerProfile Type);
	void ClearAliveDwellers();

	UFUNCTION()
	void OnDwellerDeath(AActor* DwellerActor);
	void AddDweller(ADweller* Dweller);
	void RemoveDweller(ADweller* Dweller);

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
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="SpawnerEvents")
	FLevelStart OnLevelStart;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLevelEnd);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="SpawnerEvents")
	FLevelEnd OnLevelEnd;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaveStart);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="SpawnerEvents")
	FWaveStart OnWaveStart;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWaveEnd);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="SpawnerEvents")
	FWaveEnd OnWaveEnd;
};
