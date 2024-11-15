// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerData.h"
#include "Components/BoxComponent.h"
#include "PlayerCombat.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	
protected:
	/*The max amount of queued attacks*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	int maxAttackBufferCapacity = 1;

	//SLASH
	UPROPERTY( EditAnywhere, meta = (UseComponentPicker))
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

	//Thrust
	UPROPERTY( EditAnywhere, meta = (UseComponentPicker))
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
	
	// Called when the game starts
	virtual void BeginPlay() override;
	TArray<UPrimitiveComponent*> GetAttackColliders(BufferableAttack attack);
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnSlashInput();
	void OnThrustInput();
	void OnPossessInput();
};
