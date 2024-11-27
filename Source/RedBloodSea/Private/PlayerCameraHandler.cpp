// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCameraHandler.h"

#include "PlayerData.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"

// Sets default values for this component's properties
UPlayerCameraHandler::UPlayerCameraHandler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerCameraHandler::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerCameraHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	playerCharacter->GetMesh()->SetRelativeRotation(FRotator(playerCharacter->GetControlRotation().Pitch, 0, 0));
	
	CameraRoll();
	CameraFOV();
}

void UPlayerCameraHandler::SetupPlayerCameraComponent(ACharacter* PlayerCharacter, UCameraComponent* PlayerCameraComponent)
{
	playerCharacter = PlayerCharacter;
	playerCameraComponent = PlayerCameraComponent;
	playerCameraComponent->AttachToComponent(playerCharacter->GetMesh(), FAttachmentTransformRules::KeepWorldTransform, TEXT("cameraSocket"));
}

void UPlayerCameraHandler::OnLookInput(const FVector2D newLookInput)
{
	if (playerCharacter->Controller != nullptr)
	{
		// add yaw and pitch input to controller
		playerCharacter->AddControllerYawInput(newLookInput.X);
		playerCharacter->AddControllerPitchInput(newLookInput.Y);
	}
}

void UPlayerCameraHandler::CameraRoll()
{
	//Roll

	float targetRoll = cameraRollStrength *  PlayerData::CurrentMovementInput.X;

	// Get the current controller roll input
	float currentRoll = playerCharacter->GetControlRotation().Roll;

	// Calculate the lerp alpha value based on the interpolation speed
	float lerpAlpha = FMath::Clamp(cameraRollSpeed * GetWorld()->GetDeltaSeconds(), 0.0f, 1.0f);

	// Adjust the target roll to be within the range [-180, 180] for correct interpolation
	float adjustedTargetRoll = FRotator::NormalizeAxis(targetRoll - currentRoll) + currentRoll;

	// Interpolate between the current roll and the adjusted target roll
	float newRoll = FMath::Lerp(currentRoll, adjustedTargetRoll, lerpAlpha);

	// Set the controller roll input to the new roll value
	playerCharacter->AddControllerRollInput(newRoll - currentRoll);
}

void UPlayerCameraHandler::CameraFOV()
{
	 //FOV
	 float targetFOV = PlayerData::IsDashing ? dashFOV : defaultFOV;

	 // Get the current controller roll input
	 float currentFOV = playerCameraComponent->FieldOfView;

	 // Calculate the lerp alpha value based on the interpolation speed
	 float lerpAlpha = FMath::Clamp(fovChangeSpeed * GetWorld()->GetDeltaSeconds(), 0.0f, 1.0f);

	 // Interpolate between the current roll and the adjusted target roll
	 float newFOV = FMath::Lerp(currentFOV, targetFOV, lerpAlpha);

	 playerCameraComponent->SetFieldOfView(newFOV);
}
