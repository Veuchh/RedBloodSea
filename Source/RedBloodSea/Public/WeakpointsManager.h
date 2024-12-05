// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CountLevelInstanceSubsystem.h"
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

	// WEAKPOINTS VARIABLES
	UPROPERTY(EditAnywhere, Category="Weakpoints Handler")
	const TObjectPtr<UWeakpointData> WeakpointData;
	UPROPERTY(EditAnywhere, Category="Weakpoints Handler",meta=(Bitmask,BitmaskEnum = EWeakpointType))
	uint8 TypeFilter;
	UPROPERTY(EditAnywhere, Category="Weakpoints Handler",meta=(ArraySizeEnum="EWeakpointSize"))
	int SizeNumber[static_cast<int>(EWeakpointSize::NUM)];
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weakpoints Handler",EditFixedSize)
	TArray<FName> UsedWeakpointsSocketsNames;

private:
	AActor* Owner;
	TObjectPtr<UCountLevelInstanceSubsystem> CountSubSys;
	TObjectPtr<USkeletalMeshComponent> Skeleton;
	TArray<TObjectPtr<UMaterialInstanceDynamic>> MaterialInstances;
	TArray<TObjectPtr<AWeakpoint>> Weakpoints;
	int HealthPoint;
	
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// WEAKPOINTS FUNCTIONS
	void CreateWeakPoints();
	void AttachWeakpoint(const FWeakpointSlot& WeakpointSlot,const float Size);
	UFUNCTION(BlueprintCallable)
	void RevealWeakpoints();
	UFUNCTION(BlueprintCallable)
	void RemoveWeakpoint(AWeakpoint* weakpoint);
	void ClearAllWeakpoints();
	AWeakpoint* GetFirstRevealWeakpoint();
	
	// COMPONENTS FUNCTIONS
	void SetSkeleton(USkeletalMeshComponent* skeleton);
	void SetMaterials(const TArray<TObjectPtr<UMaterialInstanceDynamic>>& newMaterialInstances);
	
	// GAMEPLAY FUNCTIONS
	bool CheckIfDead();
	int GetHealthPoint();

	// EVENTS
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeakpointReveal);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="Weakpoints Handler")
	FWeakpointReveal OnWeakpointReveal;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeakpointHit, AActor*, Weakpoint);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="Weakpoints Handler")
	FWeakpointHit OnWeakpointHit;
		
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDeath);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="Weakpoints Handler")
	FDeath OnDeath;
};
