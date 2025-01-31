// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIStatesC.h"
#include "Dweller.h"
#include "GameFramework/Actor.h"
#include "DwellerLinkSubsystem.h"
#include "WaveSpawnManager.generated.h"


UENUM(BlueprintType)
enum class EWaveType : uint8
{
	SURVIVE			UMETA(DisplayName="SURVIVE"),
	CLEAR_ALL		UMETA(DisplayName="CLEAR_ALL"),
	CHECKPOINT		UMETA(DisplayName="CHECKPOINT"),
	CLEAR_SOME		UMETA(DisplayName="CLEAR_SOME")
};

USTRUCT(Blueprintable)
struct FDwellerProfile
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere,meta=(Bitmask,BitmaskEnum = EWeakpointType))
	uint8 TypeFilter = static_cast<int>(EWeakpointType::HEAD) | static_cast<int>(EWeakpointType::ARMS) | static_cast<int>(EWeakpointType::LEGS) | static_cast<int>(EWeakpointType::TORSO);
	UPROPERTY(EditAnywhere,meta=(ArraySizeEnum="EWeakpointSize"))
	int SizeNumber[static_cast<int>(EWeakpointSize::NUM)] = {1,1,1};
	UPROPERTY(EditAnywhere)
	bool bIsAntagonist = false;
	UPROPERTY(EditAnywhere)
	EAIStatesC State = EAIStatesC::Melee;
}; UMETA(DisplayName="Wave")

USTRUCT(Blueprintable)
struct FWave
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EWaveType Type;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bTimeLimit;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Duration;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<AActor> BeginWaveTriggerZone;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<AActor> CheckpointTriggerZone;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int DwellerToRemove;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int DwellerKilled;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int DwellerLinked;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TMap<TObjectPtr<AActor>,bool> GatingBefore;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TMap<TObjectPtr<AActor>,bool> GatingDurring;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	bool bWaveCleared = false;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TMap<TObjectPtr<AActor>,FDwellerProfile> DwellerProfiles;
	
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess = "true"))
	FDateTime StartTime;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,meta=(AllowPrivateAccess = "true"))
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

public:
	TArray<ADweller*> GetAliveDwellers() const
	{
		return AliveDwellers;
	}

private:
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

	FTimerHandle CurrentWaveTimer;

	UDwellerLinkSubsystem* dwellerLinkSU;
	
	bool bIsActive = false;
	
protected:
	TQueue<TTuple<TObjectPtr<AActor>,FDwellerProfile>,EQueueMode::Spsc> SpawnQueue;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void WavePrepare();
	UFUNCTION(BlueprintCallable)
	void WaveStart();
	UFUNCTION(BlueprintCallable)
	void WaveEnd();
	UFUNCTION(BlueprintCallable)
	void WaveReset();
	UFUNCTION(BlueprintCallable)
	void WaveFail();

	
	void QueueWave(int WaveNumber);
	void SpawnDweller(FTransform Transform, FDwellerProfile Type);
	void ClearAliveDwellers(bool RemovePlayerDweller = true);
	bool CheckObjectives();

	UFUNCTION()
	void OnDwellerDeath(AActor* DwellerActor);
	UFUNCTION()
	void OnDwellerLinked(AActor* Actor);
	void AddDweller(ADweller* Dweller);
	void RemoveDweller(ADweller* Dweller,bool bRemoveFromArray = true);

	UFUNCTION()
	void OnBeginTriggerOverlap(AActor* OverlapedActor, AActor* OtherActor);
	UFUNCTION()
	void OnCheckpointBeginOverlap(AActor* OverlapedActor, AActor* OtherActor);

#if WITH_EDITOR
	//Editor functions
private:
	UFUNCTION(CallInEditor,Category="DwellerSpawners")
	void CreateSpawner();
	UFUNCTION(CallInEditor,Category="DwellerSpawners")
	void ClearSpawners();
	void PostEditChangeProperty(struct FPropertyChangedEvent& e);
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
	FWaveEnd OnWaveFail;
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="SpawnerEvents")
	FWaveEnd OnWaveSuccess;
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateObjective,const FWave&, Wave);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="SpawnerEvents")
	FUpdateObjective OnUpdateObjective;
	
};
