// Copyright Epic Games, Inc. All Rights Reserved.

#include "RedBloodSeaCharacter.h"
#include "RedBloodSeaProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
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

	SetNewPlayerSpeedAndAcceleration(defaultSpeed, defaultAcceleration);
	SetNewPlayerGravity(defaultGravity);
}

void ARedBloodSeaCharacter::Tick(float deltaTime)
{
	// Call the base class  
	Super::Tick(deltaTime);

	CameraRoll();

	if (isDashing)
		Dash();
}

void ARedBloodSeaCharacter::CameraRoll()
{
	float targetRoll = cameraRollStrength * cameraRollRotation;

	// Get the current controller roll input
	float currentRoll = GetControlRotation().Roll;

	// Calculate the lerp alpha value based on the interpolation speed
	float lerpAlpha = FMath::Clamp(cameraRollSpeed * GetWorld()->GetDeltaSeconds(), 0.0f, 1.0f);

	// Adjust the target roll to be within the range [-180, 180] for correct interpolation
	float adjustedTargetRoll = FRotator::NormalizeAxis(targetRoll - currentRoll) + currentRoll;

	// Interpolate between the current roll and the adjusted target roll
	float newRoll = FMath::Lerp(currentRoll, adjustedTargetRoll, lerpAlpha);

	// Set the controller roll input to the new roll value
	AddControllerRollInput(newRoll - currentRoll);
}

//////////////////////////////////////////////////////////////////////////// Input

void ARedBloodSeaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARedBloodSeaCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARedBloodSeaCharacter::Look);

		//Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &ARedBloodSeaCharacter::DashInput);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ARedBloodSeaCharacter::Move(const FInputActionValue& Value)
{
	if (isDashing)
	{
		return;
	}

	// input is a Vector2D
	currentMovementInput = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), currentMovementInput.Y);
		AddMovementInput(GetActorRightVector(), currentMovementInput.X);
		cameraRollRotation = currentMovementInput.X;
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::SanitizeFloat(currentMovementInput.X));
	}
}

void ARedBloodSeaCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ARedBloodSeaCharacter::DashInput(const FInputActionValue& Value)
{
	//if dash is on cooldown
	if (nextAllowedDash > UGameplayStatics::GetRealTimeSeconds(GetWorld()))
	{
		return;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::SanitizeFloat(dashEndTime));
	dashEndTime = UGameplayStatics::GetRealTimeSeconds(GetWorld()) + dashDuration;
	nextAllowedDash = UGameplayStatics::GetRealTimeSeconds(GetWorld()) + dashCooldown;
	isDashing = true;
	SetNewPlayerSpeedAndAcceleration(dashSpeed, dashAcceleration);
	//reset velocity
	GetCharacterMovement()->Velocity = (FVector::Zero());
	SetNewPlayerGravity(dashGravity);
	currentMovementInput.Normalize();
}

void ARedBloodSeaCharacter::Dash()
{
	if (!currentMovementInput.IsZero())
	{
		AddMovementInput(GetActorForwardVector(), currentMovementInput.Y);
		AddMovementInput(GetActorRightVector(), currentMovementInput.X);
	}
	else
	{
		AddMovementInput(GetActorForwardVector(), 1);
		AddMovementInput(GetActorRightVector(), 0);
	}

	//When dash over
	if (UGameplayStatics::GetRealTimeSeconds(GetWorld()) >= dashEndTime)
	{
		SetNewPlayerSpeedAndAcceleration(defaultSpeed, defaultAcceleration);
		SetNewPlayerGravity(defaultGravity);
		isDashing = false;

		GetCharacterMovement()->Velocity *= dashVelocityRemain;
	}
}

void ARedBloodSeaCharacter::SetNewPlayerSpeedAndAcceleration(float newSpeed, float newAcceleration)
{
	GetCharacterMovement()->MaxWalkSpeed = newSpeed;
	GetCharacterMovement()->MaxAcceleration = newAcceleration;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "New speed : " + FString::SanitizeFloat(newSpeed) + " New acceleration : " + FString::SanitizeFloat(newAcceleration));
}

void ARedBloodSeaCharacter::SetNewPlayerGravity(float newGravity)
{
	GetCharacterMovement()->GravityScale = newGravity;
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "New gravity : " + FString::SanitizeFloat(newGravity));
}
