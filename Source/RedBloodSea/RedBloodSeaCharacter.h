// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "RedBloodSeaCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class ARedBloodSeaCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;


	/** Dash Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	/*The default speed of the player, applied when not dashing, for instance*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Default Movement")
	float defaultSpeed = 1000;

	/*The default acceleration of the player, applied when not dashing, for instance*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Default Movement")
	float defaultAcceleration = 5000;

	/*The default gravity applied to the player, applied when not dashing, for instance*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Default Movement")
	float defaultGravity = 50;

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

	/*How much (in degrees) the camera will roll to the left or right when moving to the side*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Camera")
	float cameraRollStrength;

	/*How fast the camera will roll to the left or right when moving to the side*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Camera")
	float cameraRollSpeed;

	FVector2D currentMovementInput;
	bool isDashing = false;
	float dashEndTime;
	float nextAllowedDash = std::numeric_limits<float>::min();

public:
	ARedBloodSeaCharacter();

protected:
	virtual void BeginPlay();
	virtual void Tick(float deltaTime);
	void CameraRoll();

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

protected:
	/** Called for movement input */
	void MoveInput(const FInputActionValue& Value);

	/** Called for looking input */
	void LookInput(const FInputActionValue& Value);

	void DashInput(const FInputActionValue& Value);

	void Dash();

	void SetNewPlayerSpeedAndAcceleration(float newSpeed, float newAcceleration);

	void SetNewPlayerGravity(float newGravity);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

};

