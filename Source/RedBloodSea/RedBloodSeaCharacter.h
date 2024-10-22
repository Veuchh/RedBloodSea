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

UCLASS(config=Game)
class ARedBloodSeaCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;


	/** Dash Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float cameraRollStrength;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float cameraRollSpeed;

	float cameraRollRotation;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float dashDuration = .2f;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float dashSpeed = 5000;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float defaultSpeed = 1000;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float defaultAcceleration = 5000;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float dashAcceleration = 5000;

	bool isDashing = false;
	float dashEndTime;

	FVector2D currentMovementInput;
	
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
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);
	
	void DashInput(const FInputActionValue& Value);

	void Dash();

	void SetNewPlayerSpeedAndAcceleration(float newSpeed, float newAcceleration);

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

