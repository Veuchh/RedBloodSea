// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCameraHandler.h"

#include "PlayerData.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "WeakpointsManager.h"

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
void UPlayerCameraHandler::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red,
	                                 "CurrentWeakpointsInGlobalList : " + FString::FromInt(
		                                 UWeakpointsManager::GlobalWeakpointList.Num()));

	playerCharacter->GetMesh()->SetRelativeRotation(FRotator(playerCharacter->GetControlRotation().Pitch, 0, 0));

	CameraRoll();
	CameraFOV();
	AimAssist();
}

void UPlayerCameraHandler::SetupPlayerCameraComponent(ACharacter* PlayerCharacter,
                                                      UCameraComponent* PlayerCameraComponent,
                                                      USceneComponent* newCameraRollTarget)
{
	playerCharacter = PlayerCharacter;
	playerCameraComponent = PlayerCameraComponent;
	playerCameraComponent->AttachToComponent(playerCharacter->GetMesh(), FAttachmentTransformRules::KeepWorldTransform,
	                                         TEXT("cameraSocket"));
	cameraRollTarget = newCameraRollTarget;
}

void UPlayerCameraHandler::OnLookInput(const FVector2D newLookInput)
{
	if (PlayerData::CanRotateCamera() && playerCharacter->Controller != nullptr)
	{
		// add yaw and pitch input to controller
		playerCharacter->AddControllerYawInput(newLookInput.X);
		playerCharacter->AddControllerPitchInput(newLookInput.Y);
	}
}

void UPlayerCameraHandler::CameraRoll()
{
	//Roll

	float targetRoll = cameraRollStrength * PlayerData::CurrentMovementInput.X;

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

	FRotator newCameraRotation = FRotator(cameraRollTarget->GetRelativeRotation().Pitch,
	                                      cameraRollTarget->GetRelativeRotation().Yaw,
	                                      playerCharacter->GetControlRotation().Roll);
	cameraRollTarget->SetRelativeRotation(newCameraRotation);
}

void UPlayerCameraHandler::CameraFOV()
{
	//FOV
	float targetFOV = PlayerData::CurrentPossessState == PlayerPossessState::ZoomingCamera
		                  ? possessFOV
		                  : PlayerData::IsDashing
		                  ? dashFOV
		                  : defaultFOV;

	// Get the current controller roll input
	float currentFOV = playerCameraComponent->FieldOfView;

	// Calculate the lerp alpha value based on the interpolation speed
	float lerpAlpha = FMath::Clamp(fovChangeSpeed * GetWorld()->GetDeltaSeconds(), 0.0f, 1.0f);

	// Interpolate between the current roll and the adjusted target roll
	float newFOV = FMath::Lerp(currentFOV, targetFOV, lerpAlpha);

	playerCameraComponent->SetFieldOfView(newFOV);
}

void UPlayerCameraHandler::AimAssist()
{
	GetAimAssistTarget();
}

//This returns nullptr if no suitable weakpoint is found
AWeakpoint* UPlayerCameraHandler::GetAimAssistTarget()
{
	AWeakpoint* bestWeakpoint = nullptr;
	float bestDistanceFromScreenCenter = std::numeric_limits<float>::max();

	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red,
	                                 "Interating through " + FString::FromInt(
		                                 UWeakpointsManager::GlobalWeakpointList.Num()) + " Items");

	for (AWeakpoint* Weakpoint : UWeakpointsManager::GlobalWeakpointList)
	{
		double distanceFromCamera = (Weakpoint->GetActorLocation() - playerCameraComponent->GetComponentLocation()).
			Length();
		
		if (Weakpoint->State == EWeakpointState::Revealed)
			GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red, FString::SanitizeFloat(distanceFromCamera) + " < " + FString::SanitizeFloat(aimAssistMaxDistanceFromWeakpoint)  + FString((distanceFromCamera < aimAssistMaxDistanceFromWeakpoint) ? " = true" : " = false"));
		
		if (Weakpoint->State == EWeakpointState::Revealed
			&& distanceFromCamera < aimAssistMaxDistanceFromWeakpoint)
		{
			FVector2D d = FVector2d::ZeroVector;
			playerCharacter->GetLocalViewingPlayerController()->ProjectWorldLocationToScreen(
				Weakpoint->GetActorLocation(), d, true);
			GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red, *d.ToString());
		}
	}

	return bestWeakpoint;
}
