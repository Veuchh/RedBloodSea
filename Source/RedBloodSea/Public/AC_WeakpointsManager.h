// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AC_WeakpointsManager.generated.h"

UENUM(BlueprintType)
enum class EWeakpointType : uint8 {
	WPE_1		 UMETA(DisplayName="WPE_1"),
	WPE_2        UMETA(DisplayName="WPE_2"),
	WPE_3        UMETA(DisplayName="WPE_3"),
};

USTRUCT(BlueprintType)
struct FWeakPoint
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeakpointType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MaxOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsUsed;
}; UMETA(DisplayName="Weakpoint")

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REDBLOODSEA_API UAC_WeakpointsManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAC_WeakpointsManager();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weakpoints")
	UClass* Weakpoint_BP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weakpoints")
	int Nb_weakpoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weakpoints")
	TArray<FWeakPoint> WeakpointsSockets;

private:
	AActor* owner;
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void CreateWeakPoints();
	
		
};
