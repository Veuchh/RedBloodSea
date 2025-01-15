// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerData.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include <Kismet/GameplayStatics.h>
#include "PlayerMovement.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDashStart, FVector2D, dashDirection);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGroundSlamStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFootstep);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDashInterrupted);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class REDBLOODSEA_API UPlayerMovement : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerMovement();

protected:
	virtual void BeginPlay() override;

private:
	void SetNewPlayerSpeedAndAcceleration(float newSpeed, float newAcceleration);
	void SetNewPlayerGravity(float newGravity);
	void DashLogic();

	/*The default speed of the player, applied when not dashing, for instance*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Default Movement")
	float defaultSpeed = 1000;

	/*The default acceleration of the player, applied when not dashing, for instance*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Default Movement")
	float defaultAcceleration = 5000;

	/*The multiplier applied to the speed when in possess mode*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Default Movement")
	float possessSpeedMultiplier = .3;

	/*The default gravity applied to the player, applied when not dashing, for instance*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Default Movement")
	float defaultGravity = 50;

	/*The max amount of time an airborne player can dash before touching the ground*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Dash")
	int  maxDashInAir = 1;

	/*The duration of a dash, in seconds*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Dash")
	float dashDuration = .2f;

	/*The target speed of the player during a dash*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Dash")
	float dashSpeed = 5000;

	/*The acceleration of the player during a dash (or how fast he'll reach his target speed)*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Dash")
	float dashAcceleration = 5000;

	/*How much of the velocity the player will keep after the dash (from 0 to 1) /!\ TEST DASHING BOTH ON GROUND AND IN THE AIR AFTER MODIFYING /!\*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Dash")
	float dashVelocityRemain = .3f;

	/*The gravity applied to the player during a dash*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Dash")
	float dashGravity = 0;

	/*The amount of time, in seconds, that the player has to wait to be able to perform another dash.
	This countdown starts when pressing the dash button, not when the dash is over.*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Dash")
	float dashCooldown = .5f;

	/*How much of the velocity the player will keep after the ground slam (from 0 to 1)*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Ground Slam")
	float groundSlamHorizontalVelocityRemain = .3f;

	/*How strong the player will slam downwards*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Ground Slam")
	float groundSlamVerticalStrength = 50;

	ACharacter* playerCharacter;
	float lastTickVerticalVelocity = 0;
	bool wasPlayerGrounded = true;

public:
	void SetupPlayerMovementComponent(ACharacter* playerPawn);
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void OnNewMoveInput(FVector2D newMoveInput);
	void OnDashInput();
	void OnJumpInput(bool isJumping);
	void OnGroundSlamInput();
	void RefreshDash();

	UPROPERTY(BlueprintAssignable, Category = "Movement")
	FOnDashStart OnDashStart;
	UPROPERTY(BlueprintAssignable, Category = "Movement")
	FOnGroundSlamStart OnGroundSlamStart;
	UPROPERTY(BlueprintAssignable, Category = "Movement")
	FOnFootstep OnStartFootsteps;
	UPROPERTY(BlueprintAssignable, Category = "Movement")
	FOnDashInterrupted OnDashInterrupted;
};
