// Copyright Epic Games, Inc. All Rights Reserved.

#include "RedBloodSeaCharacter.h"
#include "RedBloodSeaProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerMovement.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include <Kismet/GameplayStatics.h>
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// ARedBloodSeaCharacter

ARedBloodSeaCharacter::ARedBloodSeaCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void ARedBloodSeaCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	UPlayerMovement* playerMovement = this->GetComponentByClass<UPlayerMovement>();
	playerMovement->SetupPlayerMovementComponent(this);
}

void ARedBloodSeaCharacter::Tick(float deltaTime)
{
	// Call the base class  
	Super::Tick(deltaTime);

	CameraRoll();
	CameraFOV();
	}

void ARedBloodSeaCharacter::CameraRoll()
{
	////Roll

	//float targetRoll = cameraRollStrength * currentMovementInput.X;

	//// Get the current controller roll input
	//float currentRoll = GetControlRotation().Roll;

	//// Calculate the lerp alpha value based on the interpolation speed
	//float lerpAlpha = FMath::Clamp(cameraRollSpeed * GetWorld()->GetDeltaSeconds(), 0.0f, 1.0f);

	//// Adjust the target roll to be within the range [-180, 180] for correct interpolation
	//float adjustedTargetRoll = FRotator::NormalizeAxis(targetRoll - currentRoll) + currentRoll;

	//// Interpolate between the current roll and the adjusted target roll
	//float newRoll = FMath::Lerp(currentRoll, adjustedTargetRoll, lerpAlpha);

	//// Set the controller roll input to the new roll value
	//AddControllerRollInput(newRoll - currentRoll);
}

void ARedBloodSeaCharacter::CameraFOV()
{
	////FOV
	//float targetFOV = isDashing ? dashFOV : defaultFOV;

	//// Get the current controller roll input
	//float currentFOV = FirstPersonCameraComponent->FieldOfView;

	//// Calculate the lerp alpha value based on the interpolation speed
	//float lerpAlpha = FMath::Clamp(fovChangeSpeed * GetWorld()->GetDeltaSeconds(), 0.0f, 1.0f);

	//// Interpolate between the current roll and the adjusted target roll
	//float newFOV = FMath::Lerp(currentFOV, targetFOV, lerpAlpha);

	//FirstPersonCameraComponent->SetFieldOfView(newFOV);
}


//////////////////////////////////////////////////////////////////////////// Input

void ARedBloodSeaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputHandler = this->GetComponentByClass<UPlayerInputHandler>();
	PlayerInputHandler->SetupPlayerInputComponent(PlayerInputComponent);
}

//void ARedBloodSeaCharacter::MoveInput(const FInputActionValue& Value)
//{
//	if (isDashing)
//	{
//		return;
//	}
//
//	// input is a Vector2D
//	currentMovementInput = Value.Get<FVector2D>();
//
//	if (Controller != nullptr)
//	{
//		// add movement 
//		AddMovementInput(GetActorForwardVector(), currentMovementInput.Y);
//		AddMovementInput(GetActorRightVector(), currentMovementInput.X);
//	}
//}
//
//void ARedBloodSeaCharacter::LookInput(const FInputActionValue& Value)
//{
//	// input is a Vector2D
//	FVector2D LookAxisVector = Value.Get<FVector2D>();
//
//	if (Controller != nullptr)
//	{
//		// add yaw and pitch input to controller
//		AddControllerYawInput(LookAxisVector.X);
//		AddControllerPitchInput(LookAxisVector.Y);
//	}
//}
