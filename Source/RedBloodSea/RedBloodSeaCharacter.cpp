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
}

void ARedBloodSeaCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	UPlayerMovement* playerMovement = this->GetComponentByClass<UPlayerMovement>();
	playerMovement->SetupPlayerMovementComponent(this);

	UPlayerCameraHandler* playerCameraHandler = this->GetComponentByClass<UPlayerCameraHandler>();
	playerCameraHandler->SetupPlayerCameraComponent(this, FirstPersonCameraComponent, GetMesh());
	
	UPlayerPossess* playerPossess = this->GetComponentByClass<UPlayerPossess>();
	playerPossess->SetupPlayerPossessComponent(this, FirstPersonCameraComponent);
}

void ARedBloodSeaCharacter::Tick(float deltaTime)
{
	// Call the base class  
	Super::Tick(deltaTime);
}

//////////////////////////////////////////////////////////////////////////// Input
void ARedBloodSeaCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	PlayerInputHandler = this->GetComponentByClass<UPlayerInputHandler>();
	PlayerInputHandler->SetupPlayerInputComponent(PlayerInputComponent);
}
