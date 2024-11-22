// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weakpoint.h"
#include "Components/ActorComponent.h"
#include "ACWeakpointsManager.generated.h"

UENUM(BlueprintType)
enum class EWeakpointType : uint8 {
	WPE_1		 UMETA(DisplayName="HIGH"),
	WPE_2        UMETA(DisplayName="MID"),
	WPE_3        UMETA(DisplayName="LOW"),
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
	int Nb_weakpoints;
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
	void AttachWeakpoint(const FName& socketName,const UE::Math::TVector<double>& maxOffset);
	UFUNCTION(BlueprintCallable)
	void RevealWeakpoints();
	UFUNCTION(BlueprintCallable)
	void RemoveWeakpoint(const AWeakpoint* weakpoint);
	UFUNCTION()
	TArray<FName> GetAllWeakpointsSockets();
	void SetSkeleton(USkeletalMeshComponent* skeleton);
	void SetMaterials(TArray<TObjectPtr<UMaterialInstanceDynamic>> newMaterialInstances);
		
};
