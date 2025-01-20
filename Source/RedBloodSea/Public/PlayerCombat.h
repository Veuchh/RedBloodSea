// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerData.h"
#include "Weakpoint.h"
#include "PlayerCombat.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSlashStart);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnThrustStart);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerHit, int, remainingHealth);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlashHitEnemy, AActor*, HitEnemy);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlashHitEnviro, AActor*, HitActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThrustHitWeakpoint, AWeakpoint*, HitWeakpoint);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThrustHitNoWeakpoint, AActor*, HitEnemy);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThrustHitEnviro, AActor*, HitActor);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class REDBLOODSEA_API UPlayerCombat : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerCombat();

private:
	void TryAddAttackToBuffer(BufferableAttack attackToAdd);
	void TryConsumeAttackBuffer();
	void OngoingAttackLogic();
	void ToggleAttackCollider(BufferableAttack attack, bool isToggled);

	bool wasWeakpointHitThisAttack = false;

protected:
	/*The max amount of queued attacks*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	int maxAttackBufferCapacity = 1;

	//SLASH
	UPROPERTY(EditAnywhere, meta = (UseComponentPicker))
	FComponentReference slashCollidersParentReference;
	TArray<UPrimitiveComponent*> slashColliders;

	/*How long before the slash collider become active*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float slashAttackStartupDelay = .05f;

	/*How long the slash collider are active during the attack*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float slashAttackDuration = .2f;

	/*How long before next attack after slash. Starts when the slash starts*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float slashAttackCooldown = .5f;

	/*How long before allowed to add to input buffer after slash.*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float slashInputBufferCooldown = .05f;

	//Thrust
	UPROPERTY(EditAnywhere, meta = (UseComponentPicker))
	FComponentReference thrustCollidersParentReference;
	TArray<UPrimitiveComponent*> thrustColliders;

	/*How long before the thrust collider become active*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float thrustAttackStartupDelay = .05f;

	/*How long the thrust collider are active during the attack*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float thrustAttackDuration = .2f;

	/*How long before next attack after thrust. Starts when the slash starts*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float thrustAttackCooldown = .5f;

	/*How long before allowed to add to input buffer after thrust.*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float thrustInputBufferCooldown = .05f;

	// Called when the game starts
	virtual void BeginPlay() override;
	TArray<UPrimitiveComponent*> GetAttackColliders(BufferableAttack attack);
	float GetAttackBufferCooldown(const BufferableAttack attack) const;
	void CallAttackBlueprintCallback(const BufferableAttack attack);
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* HitComp, class AActor* OtherActor,
	                    class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                    const FHitResult& SweepResult);
	void OnSlashOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor);
	void OnThrustOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor);
	void PlayerDeath();

	TArray<AActor*> currentAttackHitActors;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void OnSlashInput();
	void OnThrustInput();
	void OnGodModeToggle();

	UFUNCTION(BlueprintCallable)
	void DamagePlayer(int damageAmount);

	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnDeath OnPlayerDeath;
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnPlayerHit OnPlayerHit;
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnSlashStart OnSlashStart;
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnThrustStart OnThrustStart;
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnSlashHitEnemy OnSlashHitEnemy;
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnSlashHitEnviro OnSlashHitEnviro;
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnThrustHitWeakpoint OnThrustHitWeakpoint;
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnThrustHitNoWeakpoint OnThrustHitNoWeakpoint;
	UPROPERTY(BlueprintAssignable, Category = "Combat")
	FOnThrustHitEnviro FOnThrustHitEnviro;
};
