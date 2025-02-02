// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UDataSubsystem.h"
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
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Weakpoints Handler",EditFixedSize)
	//TArray<FName> UsedWeakpointsSocketsNames;

	static TArray<AWeakpoint*> GlobalWeakpointList;

private:
	AActor* Owner;
	TObjectPtr<UUDataSubsystem> CountSubSys;
	TObjectPtr<USkeletalMeshComponent> Skeleton;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weakpoints Handler", EditFixedSize, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<UMaterialInstanceDynamic>> MaterialInstances;
	UPROPERTY(VisibleAnywhere, Category="Weakpoints Handler",EditFixedSize,meta = (AllowPrivateAccess = "true"))
	TMap<FName,TObjectPtr<AWeakpoint>> Weakpoints;
	int HealthPoint;
	int MaxHealthPoint;
	FTimerHandle WeakpointsVisibilityWindowTimer;
	
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	// WEAKPOINTS FUNCTIONS
	void CreateWeakPoints();
	void AttachWeakpoint(const FWeakpointSlot& WeakpointSlot,const float Size);
	UFUNCTION(BlueprintCallable)
	void RevealWeakpoints();
	UFUNCTION(BlueprintCallable)
	void HideWeakpoints();
	UFUNCTION(BlueprintCallable)
	void RemoveWeakpoint(AWeakpoint* weakpoint, bool canDestroyHiddenWeakpoints = false);
	void ClearAllWeakpoints();
	AWeakpoint* GetRandomAliveWeakPoint();
	
	// COMPONENTS FUNCTIONS
	void SetSkeleton(USkeletalMeshComponent* skeleton);
	void SetMaterials(const TArray<TObjectPtr<UMaterialInstanceDynamic>>& newMaterialInstances);
	
	// GAMEPLAY FUNCTIONS
	bool CheckIfDead();
	int GetHealthPoint();
	int GetMaxHealthPoint();

	// EVENTS
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeakpointReveal);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="Weakpoints Handler")
	FWeakpointReveal OnWeakpointReveal;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWeakpointHide);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="Weakpoints Handler")
	FWeakpointReveal OnWeakpointHide;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeakpointHit, AActor*, Weakpoint);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="Weakpoints Handler")
	FWeakpointHit OnWeakpointHit;
		
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDeath,AActor*,Dweller);
	UPROPERTY(BlueprintAssignable,BlueprintCallable,Category="Weakpoints Handler")
	FDeath OnDeath;
};
