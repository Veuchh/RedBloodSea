// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMovement.h"

// Sets default values for this component's properties
UPlayerMovement::UPlayerMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UPlayerMovement::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void UPlayerMovement::DashLogic()
{
	if (!PlayerData::CurrentMovementInput.IsZero())
	{
		playerCharacter->AddMovementInput(playerCharacter->GetActorForwardVector(), PlayerData::CurrentMovementInput.Y);
		playerCharacter->AddMovementInput(playerCharacter->GetActorRightVector(), PlayerData::CurrentMovementInput.X);
	}
	else
	{
		playerCharacter->AddMovementInput(playerCharacter->GetActorForwardVector(), 1);
		playerCharacter->AddMovementInput(playerCharacter->GetActorRightVector(), 0);
	}

	//When dash over
	if (UGameplayStatics::GetRealTimeSeconds(GetWorld()) >= PlayerData::DashEndTime)
	{
		SetNewPlayerSpeedAndAcceleration(defaultSpeed, defaultAcceleration);
		SetNewPlayerGravity(defaultGravity);
		PlayerData::IsDashing = false;

		playerCharacter->GetCharacterMovement()->Velocity *= dashVelocityRemain;
	}
}

void UPlayerMovement::SetupPlayerMovementComponent(ACharacter* p_playerCharacter)
{
	playerCharacter = p_playerCharacter;

	SetNewPlayerSpeedAndAcceleration(defaultSpeed, defaultAcceleration);
	SetNewPlayerGravity(defaultGravity);
}

// Called every frame
void UPlayerMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PlayerData::IsDashing)
		DashLogic();
}

void UPlayerMovement::SetNewPlayerSpeedAndAcceleration(float newSpeed, float newAcceleration)
{
	playerCharacter->GetCharacterMovement()->MaxWalkSpeed = newSpeed;
	playerCharacter->GetCharacterMovement()->MaxAcceleration = newAcceleration;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, "New speed : " + FString::SanitizeFloat(newSpeed) + " New acceleration : " + FString::SanitizeFloat(newAcceleration));
}

void UPlayerMovement::SetNewPlayerGravity(float newGravity)
{
	playerCharacter->GetCharacterMovement()->GravityScale = newGravity;
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "New gravity : " + FString::SanitizeFloat(newGravity));
}

void UPlayerMovement::OnNewMoveInput(FVector2D newMoveInput)
{
}

void UPlayerMovement::OnDashInput()
{
	//if dash is on cooldown
	if (PlayerData::NextAllowedDash > UGameplayStatics::GetRealTimeSeconds(GetWorld()))
	{
		return;
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::SanitizeFloat(dashEndTime));
	PlayerData::DashEndTime = UGameplayStatics::GetRealTimeSeconds(GetWorld()) + dashDuration;
	PlayerData::NextAllowedDash = UGameplayStatics::GetRealTimeSeconds(GetWorld()) + dashCooldown;
	PlayerData::IsDashing = true;
	SetNewPlayerSpeedAndAcceleration(dashSpeed, dashAcceleration);
	//reset velocity
	playerCharacter->GetCharacterMovement()->Velocity = (FVector::Zero());
	SetNewPlayerGravity(dashGravity);
	PlayerData::CurrentMovementInput.Normalize();
}

void UPlayerMovement::OnGroundSlamInput()
{
	if (!playerCharacter->GetCharacterMovement()->IsFalling())
	{
		return;
	}
	playerCharacter->GetCharacterMovement()->Velocity *= groundSlamHorizontalVelocityRemain;
	playerCharacter->GetCharacterMovement()->Velocity.Z = -groundSlamVerticalStrength;
}