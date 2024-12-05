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

void UPlayerPossess::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
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
	}
	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Green, currentStateDebug);

	// ReSharper disable once CppDefaultCaseNotHandledInSwitchStatement
	// ReSharper disable once CppIncompleteSwitchStatement
	//We only cover states that have "waiting" states
	switch (PlayerData::CurrentPossessState)
	{
	case PlayerPossessState::TogglingAimMode:
		if (UGameplayStatics::GetTimeSeconds(GetWorld()) >= nextAllowedAction)
		{
			PlayerData::CurrentPossessState = PlayerPossessState::PossessAim;
		}
		break;
	case PlayerPossessState::ThrowFail:
		if (UGameplayStatics::GetTimeSeconds(GetWorld()) >= nextAllowedAction)
	{
		PlayerData::CurrentPossessState = PlayerPossessState::None;
	}
		break;
	case PlayerPossessState::ThrowTarget:
		break;
	case PlayerPossessState::ZoomingCamera:
		break;
	}
}

void UPlayerPossess::OnPossessModeInput(bool isToggled)
{
	if (isToggled && PlayerData::CanEnterPossessMode())
	{
		PlayerData::CurrentPossessState = PlayerPossessState::TogglingAimMode;
		nextAllowedAction = UGameplayStatics::GetTimeSeconds(GetWorld()) + holdDelayToEnterAimingMode;
	}
	else if (!isToggled && PlayerData::CurrentPossessState == PlayerPossessState::TogglingAimMode || PlayerData::CurrentPossessState == PlayerPossessState::PossessAim)
	{
		PlayerData::CurrentPossessState = PlayerPossessState::None;
	}
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
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, Hit.bBlockingHit ? FColor::Blue : FColor::Red, false, 5.0f, 0,
	              1.0f);

	// If the trace hit something, bBlockingHit will be true,
	// and its fields will be filled with detailed info about what was hit
	if (Hit.bBlockingHit && IsValid(Hit.GetActor()))
	{
		AActor* hitActor = Hit.GetActor();
		UPossessTarget* possessTarget = hitActor->GetComponentByClass<UPossessTarget>();
		//If we hit a target we can possess, we do.
		//Otherwise, we release the player possession
		if (possessTarget)
		{
			character->SetActorLocation(hitActor->GetActorLocation());
			possessTarget->OnPossessed();
		}
	}

	//If we hit nothing, we reset possession
	else
	{
		OnThrowRapierHitNothing.Broadcast();
		PlayerData::CurrentPossessState = PlayerPossessState::ThrowFail;
		nextAllowedAction = UGameplayStatics::GetTimeSeconds(GetWorld()) + throwFailDuration;
	}

	//If no, play rapier return anim
	//if yes, do the following :
	//remove player control
	//Play camera movement
	//Change player position
	// play the pull out rapier animation
	//restore player control
}
