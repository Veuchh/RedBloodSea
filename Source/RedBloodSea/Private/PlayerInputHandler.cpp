// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerInputHandler.h"

#include "PlayerCameraHandler.h"
#include "PlayerCombat.h"

// Sets default values for this component's properties
UPlayerInputHandler::UPlayerInputHandler()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerInputHandler::BeginPlay()
{
	Super::BeginPlay();

	playerMovement = GetOwner()->GetComponentByClass<UPlayerMovement>();
	playerCombat = GetOwner()->GetComponentByClass<UPlayerCombat>();
	playerCameraHandler = GetOwner()->GetComponentByClass<UPlayerCameraHandler>();
}


// Called every frame
void UPlayerInputHandler::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UPlayerInputHandler::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	//Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &UPlayerInputHandler::JumpInput);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this,
		                                   &UPlayerInputHandler::StopJumpInput);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &UPlayerInputHandler::MoveInput);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &UPlayerInputHandler::LookInput);

		// Looking
		EnhancedInputComponent->BindAction(GroundSlamAction, ETriggerEvent::Started, this,
		                                   &UPlayerInputHandler::GroundSlamInput);

		//Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Started, this, &UPlayerInputHandler::DashInput);

		//Attacks
		EnhancedInputComponent->BindAction(SlashAction, ETriggerEvent::Started, this, &UPlayerInputHandler::SlashInput);
		EnhancedInputComponent->BindAction(ThrustAction, ETriggerEvent::Started, this,
		                                   &UPlayerInputHandler::ThrustInput);
	}
	else
	{
		//UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void UPlayerInputHandler::MoveInput(const FInputActionValue& Value)
{
	playerMovement->OnNewMoveInput(Value.Get<FVector2D>());
}

void UPlayerInputHandler::LookInput(const FInputActionValue& Value)
{
	playerCameraHandler->OnLookInput(Value.Get<FVector2D>());
}

void UPlayerInputHandler::JumpInput(const FInputActionValue& Value)
{
	playerMovement->OnJumpInput(true);
}

void UPlayerInputHandler::StopJumpInput(const FInputActionValue& Value)
{
	playerMovement->OnJumpInput(false);
}

void UPlayerInputHandler::DashInput(const FInputActionValue& Value)
{
	playerMovement->OnDashInput();
}

void UPlayerInputHandler::GroundSlamInput(const FInputActionValue& Value)
{
	playerMovement->OnGroundSlamInput();
}

void UPlayerInputHandler::SlashInput(const FInputActionValue& Value)
{
	playerCombat->OnSlashInput();
}

void UPlayerInputHandler::ThrustInput(const FInputActionValue& Value)
{
	playerCombat->OnThrustInput();
}
