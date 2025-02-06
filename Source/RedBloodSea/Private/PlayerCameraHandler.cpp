// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCameraHandler.h"

#include "PlayerData.h"
#include "RedBloodSeaUserSettings.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "WeakpointsManager.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "Kismet/GameplayStatics.h"

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
}

void UPlayerCameraHandler::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	URedBloodSeaUserSettings::GetRedBloodSeaUserSettings()->OnUpdateCameraSettings.RemoveDynamic(
		this, &UPlayerCameraHandler::UpdateCameraSettings);
	
	URedBloodSeaUserSettings::GetRedBloodSeaUserSettings()->OnUpdateAccessSettings.RemoveDynamic(
		this, &UPlayerCameraHandler::UpdateAccessSettings);
}


// Called every frame
void UPlayerCameraHandler::TickComponent(float DeltaTime, ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	playerCharacter->GetMesh()->SetRelativeRotation(FRotator(playerCharacter->GetControlRotation().Pitch, 0, 0));

	CameraRoll();
	CameraFOV();
	AimAssist(DeltaTime);
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

	if (playerController == nullptr)
	{
		playerController = playerCharacter->GetLocalViewingPlayerController();
	}

	UpdateCameraSettings();
	URedBloodSeaUserSettings::GetRedBloodSeaUserSettings()->OnUpdateCameraSettings.AddDynamic(
	this, &UPlayerCameraHandler::UpdateCameraSettings);

	UpdateAccessSettings();
	URedBloodSeaUserSettings::GetRedBloodSeaUserSettings()->OnUpdateAccessSettings.AddDynamic(
		this, &UPlayerCameraHandler::UpdateAccessSettings);
}

void UPlayerCameraHandler::OnLookInput(const FVector2D newLookInput)
{
	if (PlayerData::CanRotateCamera() && playerCharacter->Controller != nullptr)
	{
		// add yaw and pitch input to controller
		playerCharacter->AddControllerYawInput(newLookInput.X * sensitivityMultiplier);
		playerCharacter->AddControllerPitchInput(newLookInput.Y * sensitivityMultiplier);
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
		                  ? possessFOV + defaultFOV
		                  : PlayerData::IsDashing
		                  ? dashFOV + defaultFOV
		                  : defaultFOV;

	// Get the current controller roll input
	float currentFOV = playerCameraComponent->FieldOfView;

	// Calculate the lerp alpha value based on the interpolation speed
	float lerpAlpha = FMath::Clamp((PlayerData::CurrentPossessState == PlayerPossessState::ZoomingCamera
		                                ? possessFovChangeSpeed
		                                : dashFovChangeSpeed) * GetWorld()->GetDeltaSeconds(),
	                               0.0f,
	                               1.0f);

	// Interpolate between the current roll and the adjusted target roll
	float newFOV = FMath::Lerp(currentFOV,
	                           targetFOV,
	                           lerpAlpha);

	playerCameraComponent->SetFieldOfView(newFOV);
}

void UPlayerCameraHandler::AimAssist(float deltaTime)
{
	if (PlayerData::CanRotateCamera()
		&& playerCharacter->Controller != nullptr
		&& PlayerData::CurrentPossessState == PlayerPossessState::None)
	{
		if (playerController == nullptr)
		{
			playerController = playerCharacter->GetLocalViewingPlayerController();
		}

		AWeakpoint* aimAssistTarget = GetAimAssistTarget();

		if (aimAssistTarget != nullptr)
		{
			FVector2d screenPosition = FVector2d::ZeroVector;
			playerController->ProjectWorldLocationToScreen(aimAssistTarget->GetActorLocation(), screenPosition, false);
			FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
			screenPosition = (screenPosition / ViewportSize) - (FVector2d::One() / 2);

			GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red, FString::SanitizeFloat(screenPosition.X));

			// add yaw and pitch input to controller
			playerCharacter->AddControllerYawInput(screenPosition.X * aimAssistStrength * deltaTime);
			playerCharacter->AddControllerPitchInput(screenPosition.Y * aimAssistStrength * deltaTime);
		}
	}
}

void UPlayerCameraHandler::UpdateCameraSettings()
{
	defaultFOV = URedBloodSeaUserSettings::GetRedBloodSeaUserSettings()->FieldOfView;
	sensitivityMultiplier = URedBloodSeaUserSettings::GetRedBloodSeaUserSettings()->CameraSensitivity;
}

void UPlayerCameraHandler::UpdateAccessSettings()
{
	aimAssistMultiplier = URedBloodSeaUserSettings::GetRedBloodSeaUserSettings()->AimAssistStrength;
}

//This returns nullptr if no suitable weakpoint is found
AWeakpoint* UPlayerCameraHandler::GetAimAssistTarget()
{
	AWeakpoint* bestWeakpoint = nullptr;
	float bestDistanceFromScreenCenter = std::numeric_limits<float>::max();
	FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());

	for (AWeakpoint* Weakpoint : UWeakpointsManager::GlobalWeakpointList)
	{
		double distanceFromCamera = (Weakpoint->GetActorLocation() - playerCameraComponent->GetComponentLocation()).
			Length();

		if (Weakpoint->State == EWeakpointState::Revealed
			&& distanceFromCamera < aimAssistMaxDistanceFromWeakpoint)
		{
			FVector2D screenPosition = FVector2d::ZeroVector;
			if (playerController->ProjectWorldLocationToScreen(Weakpoint->GetActorLocation(), screenPosition, false))
			{
				screenPosition = screenPosition / ViewportSize;
				float distanceToCenter = (screenPosition - (FVector2d::One() / 2)).Length();

				if (distanceToCenter < aimAssistMaxDistanceFromScreenCenter * aimAssistMultiplier
					&& distanceToCenter < bestDistanceFromScreenCenter)
				{
					bestWeakpoint = Weakpoint;
					bestDistanceFromScreenCenter = distanceToCenter;
				}
			}
		}
	}

	return bestWeakpoint;
}
