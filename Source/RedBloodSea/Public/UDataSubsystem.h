// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelInstance/LevelInstanceSubsystem.h"
#include "UDataSubsystem.generated.h"


UENUM(BlueprintType)
enum class EActionType: uint8 {
	SESSION_START		UMETA(DisplayName="SESSION_START"),
	SESSION_END			UMETA(DisplayName="SESSION_END"),
	DWELLER_KILL		UMETA(DisplayName="DWELLER_KILL"),
	DWELLER_POSSESS		UMETA(DisplayName="DWELLER_POSSESS"),
	DWELLER_LINK		UMETA(DisplayName="DWELLER_LINK"),
	DASH				UMETA(DisplayName="DASH"),
	JUMP				UMETA(DisplayName="JUMP"),
	GROUND_SLAM			UMETA(DisplayName="GROUND_SLAM"),
	THRUST_HIT			UMETA(DisplayName="THRUST_HIT"),
	THRUST_MISS			UMETA(DisplayName="THRUST_MISS"),
	SLASH_HIT			UMETA(DisplayName="SLASH_HIT"),
	SLASH_MISS			UMETA(DisplayName="SLASH_MISS"),
	THROW_HIT			UMETA(DisplayName="THROW_HIT"),
	THROW_MISS			UMETA(DisplayName="THROW_MISS"),
	DAMAGED				UMETA(DisplayName="DAMAGED"),
	DEATH				UMETA(DisplayName="DEATH"),
	MAX					UMETA(Hidden)
};
ENUM_RANGE_BY_COUNT(EActionType, EActionType::MAX)


USTRUCT()
struct FActionLog
{
	GENERATED_BODY()
	FDateTime TimeStamp;
	EActionType Type;
	FVector Position;
};
/**
 * 
 */
UCLASS()
class REDBLOODSEA_API UUDataSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	//DataLogs Variables
	FDateTime LogStart;
	TArray<FActionLog> Logs;


	
	//Count Variables
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TMap<EActionType,int> Counters;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int KillCount;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int LinkedCount;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable)
	void StartLogs();
	UFUNCTION(BlueprintCallable)
	void StopLogs();
	UFUNCTION(BlueprintCallable)
	void ResetLogs();

	UFUNCTION(BlueprintCallable)
	bool ExportLogs(FString FolderLocation, FString FileName);
	

	UFUNCTION(BlueprintCallable)
	float GetKillPercent();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKill);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="Count")
	FKill OnKill;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FLinked,int,count);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="Count")
	FLinked OnLinked;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountChange,float,killPercent);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="Count")
	FCountChange OnCountChange;

	UFUNCTION(BlueprintCallable)
	void LogEventAtPos(const EActionType Type, const FVector& Position);

	UFUNCTION(BlueprintCallable)
	void LogEvent(const EActionType Type);
	

	
	UFUNCTION(BlueprintCallable)
	void AddKill();
	UFUNCTION(BlueprintCallable)
	void AddLinked(int Count);

	UFUNCTION(BlueprintCallable)
	void ResetAll();

private:
	FString GetActionName(const EActionType Type);
	TArray<FString> LogsToStrings();
};
