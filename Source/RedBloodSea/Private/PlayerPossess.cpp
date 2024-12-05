// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPossess.h"

#include "PossessTarget.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPlayerPossess::UPlayerPossess()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UPlayerPossess::SetupPlayerPossessComponent(ACharacter* Character,
                                                 UCameraComponent* CameraComponent)
{
	character = Character;
	camera = CameraComponent;
}

void DebugState()
{
	FString currentStateDebug = "";

	switch (PlayerData::CurrentPossessState)
	{
	case PlayerPossessState::None:
		currentStateDebug = "None";
		break;
	case PlayerPossessState::TogglingAimMode:
		currentStateDebug = "TogglingPossessAim";
		break;
	case PlayerPossessState::PossessAim:
		currentStateDebug = "PossessAim";
		break;
	case PlayerPossessState::ThrowFail:
		currentStateDebug = "ThrowFail";
		break;
	case PlayerPossessState::ThrowTarget:
		currentStateDebug = "ThrowTarget";
		break;
	case PlayerPossessState::ZoomingCamera:
		currentStateDebug = "ZoomingCamera";
		break;
	case PlayerPossessState::PossessRecovery:
		currentStateDebug = "PossessRecovery";
		break;
	}
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, currentStateDebug);
}

void UPlayerPossess::CameraZoomTick()
{
	if (UGameplayStatics::GetTimeSeconds(GetWorld()) <= nextAllowedAction)
	{
		//We progressively move the player towards its target
		float alpha = FMath::GetRangePct(
			PlayerData::StartCameraMovementTime,
			PlayerData::EndCameraMovementTime,
			UGameplayStatics::GetTimeSeconds(GetWorld()));


		//We do alpha * alpha here to have an exponentially faster movement
		FVector currentPosition = FMath::Lerp(startPossessPosition, endPossessPosition, alpha * alpha);
		
		character->SetActorLocation(currentPosition);
	}
	else
	{
		PlayerData::CurrentPossessState = PlayerPossessState::PossessRecovery;
		nextAllowedAction = UGameplayStatics::GetTimeSeconds(GetWorld()) + playerControlDelay;
		
		character->SetActorLocation(PlayerData::CurrentPossessTarget->GetOwner()->GetActorLocation());

		PlayerData::CurrentPossessTarget->Possess();

		TogglePlayer(true);
	}
}

void UPlayerPossess::ThrowTargetTick()
{
	if (UGameplayStatics::GetTimeSeconds(GetWorld()) >= nextAllowedAction)
	{
		PlayerData::CurrentPossessState = PlayerPossessState::ZoomingCamera;
		PlayerData::StartCameraMovementTime = UGameplayStatics::GetTimeSeconds(GetWorld());
		PlayerData::EndCameraMovementTime = PlayerData::StartCameraMovementTime + possessTransitionDuration;
		nextAllowedAction = PlayerData::EndCameraMovementTime;
		TogglePlayer(false);
		startPossessPosition = GetOwner()->GetActorLocation();
		endPossessPosition = PlayerData::CurrentPossessTarget->GetOwner()->GetActorLocation();
		if (PlayerData::CurrentPossessTarget)
		{
			PlayerData::CurrentPossessTarget->Unpossess(GetOwner()->GetActorLocation());
		}
	}
}

void UPlayerPossess::ThrowFailTick()
{
	if (UGameplayStatics::GetTimeSeconds(GetWorld()) >= nextAllowedAction)
	{
		PlayerData::CurrentPossessState = PlayerPossessState::None;
	}
}

void UPlayerPossess::AimModeTogglingTick()
{
	if (UGameplayStatics::GetTimeSeconds(GetWorld()) >= nextAllowedAction)
	{
		PlayerData::CurrentPossessState = PlayerPossessState::PossessAim;
	}
}

void UPlayerPossess::PossessRecoveryTick()
{
	if (UGameplayStatics::GetTimeSeconds(GetWorld()) >= nextAllowedAction)
	{
		PlayerData::CurrentPossessState = PlayerPossessState::None;
	}
}

void UPlayerPossess::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	DebugState();

	AimModeToggling();

	//We only cover states that have "waiting" logic
	switch (PlayerData::CurrentPossessState)
	{
	case PlayerPossessState::TogglingAimMode:
		AimModeTogglingTick();
		break;
	case PlayerPossessState::ThrowFail:
		ThrowFailTick();
		break;
	case PlayerPossessState::ThrowTarget:
		ThrowTargetTick();
		break;
	case PlayerPossessState::ZoomingCamera:
		CameraZoomTick();
		break;
	case PlayerPossessState::PossessRecovery:
		PossessRecoveryTick();
		break;
	}
}

void UPlayerPossess::AimModeToggling()
{
	if (isInputModeActionPressed && PlayerData::CanEnterPossessMode())
	{
		PlayerData::CurrentPossessState = PlayerPossessState::TogglingAimMode;
		nextAllowedAction = UGameplayStatics::GetTimeSeconds(GetWorld()) + holdDelayToEnterAimingMode;
	}
	else if (!isInputModeActionPressed && (PlayerData::CurrentPossessState == PlayerPossessState::TogglingAimMode ||
		PlayerData::CurrentPossessState == PlayerPossessState::PossessAim))
	{
		PlayerData::CurrentPossessState = PlayerPossessState::None;
	}
}

void UPlayerPossess::TogglePlayer(bool isToggled) const
{
	character->SetActorEnableCollision(isToggled);
	character->GetMesh()->SetVisibility(isToggled);
}

void UPlayerPossess::OnPossessModeInput(bool isToggled)
{
	isInputModeActionPressed = isToggled;
}

void UPlayerPossess::OnPossessInput()
{
	if (!PlayerData::CanUsePossess())
	{
		return;
	}

	// FHitResult will hold all data returned by our line collision query
	FHitResult Hit;

	// We set up a line trace from our current location to a point 1000cm ahead of us
	FVector TraceStart = camera->GetComponentLocation();
	FVector TraceEnd = camera->GetComponentLocation() + camera->GetForwardVector() * maxPossessDistance;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);

	// You can use DrawDebug helpers and the log to help visualize and debug your trace queries.
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false,
	              5.0f, 0, 1.0f);

	// If the trace hit something, bBlockingHit will be true,
	// and its fields will be filled with detailed info about what was hit
	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		//If we hit a target we can possess, we do.
		//Otherwise, we release the player possession
		if (UPossessTarget* possessTarget = Hit.GetActor()->GetComponentByClass<UPossessTarget>())
		{
			PlayerData::CurrentPossessTarget = possessTarget;
			PlayerData::CurrentPossessState = PlayerPossessState::ThrowTarget;
			nextAllowedAction = UGameplayStatics::GetTimeSeconds(GetWorld()) + possessDelay;
		}
	}

	//If we hit nothing, we reset possession
	else
	{
		OnThrowRapierHitNothing.Broadcast();
		PlayerData::CurrentPossessState = PlayerPossessState::ThrowFail;
		nextAllowedAction = UGameplayStatics::GetTimeSeconds(GetWorld()) + throwFailDuration;
	}
}
