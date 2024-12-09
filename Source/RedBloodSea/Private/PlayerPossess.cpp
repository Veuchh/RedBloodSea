// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerPossess.h"

#include "PossessTarget.h"
#include "WeakpointsManager.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
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
	case PlayerPossessState::ThrowFailNotPossessing:
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
	case PlayerPossessState::ThrowFailWhilePossessing:
		currentStateDebug = "ThrowFailWhilePossessing";
		break;
	}

	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, currentStateDebug);
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Red,
	                                 PlayerData::IsPossessingBody
		                                 ? "Current Body : possessed enemy"
		                                 : "Current Body : bearer");
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

		UWeakpointsManager* wpManager = PlayerData::CurrentPossessTarget->GetOwner()->GetComponentByClass<
			UWeakpointsManager>();
		if (wpManager)
		{
			PlayerData::PossessedBodyCurrentHPAmount = wpManager->GetHealthPoint();
			PlayerData::PossessedBodyMaxHPAmount = wpManager->GetMaxHealthPoint();
		}
		PlayerData::CurrentPossessTarget->Possess();


		TogglePlayer(true);

		//Update UI for HP
		OnUpdateHPDisplay.Broadcast(PlayerData::GetCurrentHP(), PlayerData::GetMaxHP());

		OnPossessRecovery.Broadcast();
	}
}

void UPlayerPossess::SetupCameraMovement()
{
	PlayerData::StartCameraMovementTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	PlayerData::EndCameraMovementTime = PlayerData::StartCameraMovementTime + possessTransitionDuration;
	nextAllowedAction = PlayerData::EndCameraMovementTime;
	TogglePlayer(false);
	startPossessPosition = GetOwner()->GetActorLocation();
	endPossessPosition = PlayerData::CurrentPossessTarget->GetOwner()->GetActorLocation();
}

void UPlayerPossess::ThrowTargetTick()
{
	if (UGameplayStatics::GetTimeSeconds(GetWorld()) >= nextAllowedAction)
	{
		PlayerData::CurrentPossessState = PlayerPossessState::ZoomingCamera;
		SetupCameraMovement();

		if (PlayerData::IsPossessingBody)
		{
			targetToUnpossess->Unpossess(GetOwner()->GetActorLocation());
		}
		else
		{
			LeaveBearerBodyAtPosition();
		}

		PlayerData::IsPossessingBody = !PlayerData::CurrentPossessTarget->IsBearerBody;
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

void UPlayerPossess::ThrowFailWhilePossessingTick()
{
	if (UGameplayStatics::GetTimeSeconds(GetWorld()) >= nextAllowedAction)
	{
		targetToUnpossess->Unpossess(GetOwner()->GetActorLocation());
		PlayerData::CurrentPossessState = PlayerPossessState::ZoomingCamera;
		PlayerData::IsPossessingBody = false;
		SetupCameraMovement();
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

	//We only cover states that have "waiting for cooldown" logic (not those waiting for a player input)
	switch (PlayerData::CurrentPossessState)
	{
	case PlayerPossessState::TogglingAimMode:
		AimModeTogglingTick();
		break;
	case PlayerPossessState::ThrowFailNotPossessing:
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
	case PlayerPossessState::ThrowFailWhilePossessing:
		ThrowFailWhilePossessingTick();
		break;
	}
}

void UPlayerPossess::AimModeToggling()
{
	if (isInputModeActionPressed && PlayerData::CanEnterPossessMode() && !character->GetCharacterMovement()->
		IsFalling())
	{
		OnPossessAimStart.Broadcast();
		PlayerData::CurrentPossessState = PlayerPossessState::TogglingAimMode;
		nextAllowedAction = UGameplayStatics::GetTimeSeconds(GetWorld()) + holdDelayToEnterAimingMode;
	}
	else if ((!isInputModeActionPressed || character->GetCharacterMovement()->IsFalling()) && (
		PlayerData::CurrentPossessState == PlayerPossessState::TogglingAimMode ||
		PlayerData::CurrentPossessState == PlayerPossessState::PossessAim))
	{
		OnPossessAimStop.Broadcast();
		PlayerData::CurrentPossessState = PlayerPossessState::None;
	}
}

void UPlayerPossess::TogglePlayer(bool isToggled) const
{
	character->SetActorEnableCollision(isToggled);
	character->GetMesh()->SetVisibility(isToggled);
}

void UPlayerPossess::LeaveBearerBodyAtPosition()
{
	FVector Location = GetOwner()->GetActorLocation();
	FRotator Rotation = GetOwner()->GetActorRotation();
	FActorSpawnParameters SpawnInfo;
	GetWorld()->SpawnActor<ABearerBody>(Location, Rotation, SpawnInfo);
	bearerBodyInstance = GetWorld()->SpawnActor<ABearerBody>(BearerBodyBP, Location, Rotation, SpawnInfo);
}

void UPlayerPossess::OnPossessModeInput(bool isToggled)
{
	isInputModeActionPressed = isToggled;
}

void UPlayerPossess::LineTrace(FVector TraceStart, FVector TraceEnd, FHitResult& Hit)
{
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, TraceChannelProperty, QueryParams);
}

void UPlayerPossess::OnPossessInput()
{
	if (!PlayerData::CanUsePossess())
	{
		return;
	}

	FVector TraceStart = camera->GetComponentLocation();
	FVector TraceEnd = camera->GetComponentLocation() + camera->GetForwardVector() * maxPossessDistance;

	FHitResult Hit;

	LineTrace(TraceStart, TraceEnd, Hit);

	UPossessTarget* possessTarget = nullptr;

	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		possessTarget = Hit.GetActor()->GetComponentByClass<UPossessTarget>();
	}

	//If we hit a target we can possess, we do.
	if (possessTarget)
	{
		if (PlayerData::IsPossessingBody)
		{
			targetToUnpossess = PlayerData::CurrentPossessTarget;
		}
		PlayerData::CurrentPossessTarget = possessTarget;
		PlayerData::CurrentPossessState = PlayerPossessState::ThrowTarget;
		nextAllowedAction = UGameplayStatics::GetTimeSeconds(GetWorld()) + possessDelay;
		OnThrowRapierTarget.Broadcast(Hit.ImpactPoint);
	}

	//If we hit nothing, but we are possessing an enemy, we go back into the bearer body
	else if (PlayerData::IsPossessingBody)
	{
		targetToUnpossess = PlayerData::CurrentPossessTarget;
		PlayerData::CurrentPossessState = PlayerPossessState::ThrowFailWhilePossessing;
		nextAllowedAction = UGameplayStatics::GetTimeSeconds(GetWorld()) + throwFailWhilePossessingDelay;
		PlayerData::CurrentPossessTarget = bearerBodyInstance->GetComponentByClass<UPossessTarget>();
	}

	//otherwise, we just play the throw fail animation
	else
	{
		PlayerData::CurrentPossessState = PlayerPossessState::ThrowFailNotPossessing;
		nextAllowedAction = UGameplayStatics::GetTimeSeconds(GetWorld()) + throwFailDuration;
	}

	if (Hit.bBlockingHit)
	{
		OnThrowRapierEnviro.Broadcast(Hit.ImpactPoint);
	}
	else
	{
		OnThrowRapierNothing.Broadcast(camera->GetComponentLocation() + camera->GetForwardVector() * 1000);
	}

	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false,
	              5.0f, 0, 1.0f);
}
