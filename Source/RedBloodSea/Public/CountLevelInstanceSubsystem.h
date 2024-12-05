// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelInstance/LevelInstanceSubsystem.h"
#include "CountLevelInstanceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class REDBLOODSEA_API UCountLevelInstanceSubsystem : public ULevelInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int killCount;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int possessCount;

	UFUNCTION()
	void GetKillPercent();

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FKill);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="Count")
	FKill OnKill;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPossess);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="Count")
	FPossess OnPossess;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCountChange,float,killPercent);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="Count")
	FCountChange OnCountChange;

	UFUNCTION()
	void AddKill();
	UFUNCTION()
	void AddPossess();
};
