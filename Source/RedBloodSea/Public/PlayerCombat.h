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
	
protected:
	/*The max amount of queued attacks*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Default Movement")
	int attackBufferMaxSize = 1;

	//SLASH
	UPROPERTY( EditAnywhere, meta = ( UseComponentPicker ) )
	FComponentReference slashCollidersParentReference;
	TArray<UPrimitiveComponent*> slashColliders;
	
	/*How long before the slash collider become active*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Default Movement")
	float slashAttackStartupDelay = .05f;

	/*How long the slash collider are active during the attack*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Default Movement")
	float slashAttackDuration = .2f;

	/*How long before next attack after  slash. Starts when the slash starts*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Default Movement")
	float slashAttackCooldown = .5f;
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void OnSlashInput();
	void OnThrustInput();
	void OnPossessInput();
};
