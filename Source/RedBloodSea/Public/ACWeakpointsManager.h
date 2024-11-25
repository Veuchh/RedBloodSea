// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weakpoint.h"
#include "Components/ActorComponent.h"
#include "ACWeakpointsManager.generated.h"



UENUM(BlueprintType)
enum class EWeakpointType : uint8 {
	HEAD		UMETA(DisplayName="HEAD"),
	TORSO		UMETA(DisplayName="TORSO"),
	LEGS		UMETA(DisplayName="LEGS"),
	ARMS		UMETA(DisplayName="ARMS"),
};

UENUM(BlueprintType)
enum class EWeakpointWeight : uint8 {
	HIGH		UMETA(DisplayName="HIGH"),
	MID			UMETA(DisplayName="MID"),
	LOW			UMETA(DisplayName="LOW"),
};

UENUM(BlueprintType)
enum class EWeakpointSize : uint8 {
	BIG				UMETA(DisplayName="BIG"),
	MEDIUM			UMETA(DisplayName="MEDIUM"),
	SMALL			UMETA(DisplayName="SMALL"),
};

USTRUCT(BlueprintType)
struct FWeakpointSlot
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(GetOptions="GetAllWeakpointsSockets"))
	FName SocketName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeakpointType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeakpointWeight Weight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MaxOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MinOffset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsUsed;
}; UMETA(DisplayName="Weakpoint slot")

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REDBLOODSEA_API UACWeakpointsManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UACWeakpointsManager();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weakpoints Handler")
	TObjectPtr<UClass> Weakpoint_BP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weakpoints Handler")
	TMap<EWeakpointWeight,int> WeightTable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weakpoints Handler")
	TMap<EWeakpointType,bool> TypeFilter;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weakpoints Handler")
	TMap<EWeakpointSize,float> SizeChart;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weakpoints Handler")
	TMap<EWeakpointSize,int> SizeNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weakpoints Handler")
	TArray<FWeakpointSlot> WeakpointsSockets;

private:
	AActor* owner;
	TObjectPtr<USkeletalMeshComponent> skeleton;
	TArray<TObjectPtr<UMaterialInstanceDynamic>> materialInstances;
	TArray<TObjectPtr<AWeakpoint>> weakpoints;
	
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void CreateWeakPoints();
	void AttachWeakpoint(const FWeakpointSlot& WeakpointSlot,const float Size);
	UFUNCTION(BlueprintCallable)
	void RevealWeakpoints();
	UFUNCTION(BlueprintCallable)
	void RemoveWeakpoint(AWeakpoint* weakpoint);
	//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeakpointOverlapBegin, AActor*, Weakpoint, AActor*, OtherActor);
	//UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="Weakpoints")
	//FWeakpointOverlapBegin OnWeakpointOverlapBegin;
	UFUNCTION()
	void WeakpointOverlapBegin(AActor* OverlapedActor, AActor* OtherActor);
	UFUNCTION()
	TArray<FName> GetAllWeakpointsSockets();
	void SetSkeleton(USkeletalMeshComponent* skeleton);
	void SetMaterials(TArray<TObjectPtr<UMaterialInstanceDynamic>> newMaterialInstances);
	
		
};
