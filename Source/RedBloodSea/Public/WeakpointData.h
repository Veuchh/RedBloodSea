// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeakpointData.generated.h"

/**
 * 
 */

UENUM(meta = (Bitflags, UseEnumValuesAsMaskValuesInEditor = "true"))
enum class EWeakpointType : uint8 {
	HEAD =	1 << 0		UMETA(DisplayName="HEAD"),
	TORSO = 1 << 1		UMETA(DisplayName="TORSO"),
	LEGS =	1 << 2		UMETA(DisplayName="LEGS"),
	ARMS =	1 << 3		UMETA(DisplayName="ARMS"),
	END					UMETA(Hidden)
};

UENUM()
enum class EWeakpointWeight : uint8 {
	HIGH		UMETA(DisplayName="HIGH"),
	MID			UMETA(DisplayName="MID"),
	LOW			UMETA(DisplayName="LOW"),
	NUM			UMETA(Hidden)
};

UENUM()
enum class EWeakpointSize : uint8 {
	BIG			UMETA(DisplayName="BIG"),
	MEDIUM		UMETA(DisplayName="MEDIUM"),
	SMALL		UMETA(DisplayName="SMALL"),
	NUM			UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FWeakpointSlot
{
	GENERATED_BODY()
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,meta=(GetOptions="GetAllWeakpointsSockets"))
	FName SocketName;
	UPROPERTY(EditAnywhere)
	EWeakpointType Type;
	UPROPERTY(EditAnywhere)
	EWeakpointWeight Weight;
	UPROPERTY(EditAnywhere)
	FVector MaxOffset;
	UPROPERTY(EditAnywhere)
	FVector MinOffset;
}; UMETA(DisplayName="Weakpoint slot")

UCLASS(BlueprintType)
class REDBLOODSEA_API UWeakpointData : public UDataAsset
{
	GENERATED_BODY()
public:
	UWeakpointData();
	void PostEditChangeProperty(struct FPropertyChangedEvent& e);
	
	UPROPERTY(EditAnywhere, Category="Weakpoints Handler")
	TObjectPtr<UClass> Weakpoint_BP;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weakpoints Data")
	TObjectPtr<USkeletalMesh> SkeletalMesh;
	UPROPERTY(EditAnywhere, Category="Weakpoints Data",meta=(ArraySizeEnum="EWeakpointWeight"))
	int WeightTable[static_cast<int>(EWeakpointWeight::NUM)];
	UPROPERTY(EditAnywhere, Category="Weakpoints Data",meta=(ArraySizeEnum="EWeakpointSize"))
	float SizeChart[static_cast<int>(EWeakpointSize::NUM)];
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Weakpoints Data",EditFixedSize)
	TArray<FWeakpointSlot> WeakpointsSockets;

	UFUNCTION()
	TArray<FName> GetAllWeakpointsSockets() const;


private:
	void UpdateSockets();
};
