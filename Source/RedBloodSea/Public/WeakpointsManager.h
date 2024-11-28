// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weakpoint.h"
#include "WeakpointData.h"
#include "Components/ActorComponent.h"
#include "WeakpointsManager.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REDBLOODSEA_API UWeakpointsManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeakpointsManager();
	UPROPERTY(EditAnywhere, Category="Weakpoints Handler")
	TObjectPtr<UClass> Weakpoint_BP;
	UPROPERTY(EditAnywhere, Category="Weakpoints Handler")
	const TObjectPtr<UWeakpointData> WeakpointData;
	UPROPERTY(EditAnywhere, Category="Weakpoints Handler",meta=(Bitmask,BitmaskEnum = EWeakpointType))
	uint8 TypeFilter;
	UPROPERTY(EditAnywhere, Category="Weakpoints Handler",meta=(ArraySizeEnum="EWeakpointSize"))
	int SizeNumber[static_cast<int>(EWeakpointSize::NUM)];
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weakpoints Handler",EditFixedSize)
	TArray<FName> UsedWeakpointsSocketsNames;

private:
	AActor* owner;
	TObjectPtr<USkeletalMeshComponent> skeleton;
	TArray<TObjectPtr<UMaterialInstanceDynamic>> materialInstances;
	TArray<TObjectPtr<AWeakpoint>> Weakpoints;
	
	
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
	void SetSkeleton(USkeletalMeshComponent* skeleton);
	void SetMaterials(TArray<TObjectPtr<UMaterialInstanceDynamic>> newMaterialInstances);



	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeakpointReveal, AActor*, Character);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="Weakpoints Handler")
	FWeakpointReveal OnWeakpointReveal;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWeakpointHit,AActor*, Character, AActor*, Weakpoint);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="Weakpoints Handler")
	FWeakpointHit OnWeakpointHit;
		
		
};
