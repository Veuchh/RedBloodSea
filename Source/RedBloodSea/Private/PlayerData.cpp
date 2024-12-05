#include "PlayerData.h"

FVector2D PlayerData::CurrentMovementInput = FVector2D::Zero();
bool PlayerData::IsDashing = false;
float PlayerData::DashEndTime = 0;
int PlayerData::RemainingAirDashes = 0;
float PlayerData::NextAllowedDash = 0;
int PlayerData::MaxAttackBufferCapacity = 0; //Initialized in PlayerCombat.BeginPlay
BufferableAttack PlayerData::CurrentAttack = BufferableAttack::None;
TArray<BufferableAttack> PlayerData::AttackBuffer = TArray<BufferableAttack>();
PlayerAttackState PlayerData::CurrentAttackState = PlayerAttackState::None;
float PlayerData::AttackStartTime = 0;
float PlayerData::NextAllowedInputBufferTime = 0;
PlayerPossessState PlayerData::CurrentPossessState = PlayerPossessState::None;

int PlayerData::CurrentHPAmount = 0; //Initialized in PlayerCombat.BeginPlay
int PlayerData::MaxHPAmount = 0; //Initialized in PlayerCombat.BeginPlay
float PlayerData::SlashAttackStartupDelay = 0; //Initialized in PlayerCombat.BeginPlay
float PlayerData::SlashAttackDuration = 0; //Initialized in PlayerCombat.BeginPlay
float PlayerData::SlashAttackCooldown = 0; //Initialized in PlayerCombat.BeginPlay
float PlayerData::ThrustAttackStartupDelay = 0; //Initialized in PlayerCombat.BeginPlay
float PlayerData::ThrustAttackDuration = 0; //Initialized in PlayerCombat.BeginPlay
float PlayerData::ThrustAttackCooldown = 0; //Initialized in PlayerCombat.BeginPlay

PlayerData::PlayerData()
{
}

PlayerData::~PlayerData()
{
}

void PlayerData::ResetData()
{
	CurrentMovementInput = FVector2D::Zero();
	IsDashing = false;
	DashEndTime = 0;
	RemainingAirDashes = 0;
	NextAllowedDash = 0;
	CurrentAttack = BufferableAttack::None;
	AttackBuffer = TArray<BufferableAttack>();
	CurrentAttackState = PlayerAttackState::None;
	AttackStartTime = 0;
	NextAllowedInputBufferTime = 0;
	CurrentPossessState = PlayerPossessState::None;
}


float PlayerData::GetCurrentAttackCooldownEndTime()
{
	switch (CurrentAttack)
	{
	case BufferableAttack::Slash:
		return AttackStartTime + SlashAttackStartupDelay + SlashAttackDuration + SlashAttackCooldown;
	case BufferableAttack::Thrust:
		return AttackStartTime + ThrustAttackStartupDelay + ThrustAttackDuration + ThrustAttackCooldown;
	}

	return 0;
}

float PlayerData::GetCurrentAttackStartupEndTime()
{
	switch (CurrentAttack)
	{
	case BufferableAttack::Slash:
		return AttackStartTime + SlashAttackStartupDelay;
	case BufferableAttack::Thrust:
		return AttackStartTime + ThrustAttackStartupDelay;
	}

	return 0;
}

float PlayerData::GetCurrentAttackColliderEndTime()
{
	switch (CurrentAttack)
	{
	case BufferableAttack::Slash:
		return AttackStartTime + SlashAttackStartupDelay + SlashAttackDuration;
	case BufferableAttack::Thrust:
		return AttackStartTime + ThrustAttackStartupDelay + ThrustAttackDuration;
	}

	return 0;
}

bool PlayerData::CanMove()
{
	return !IsDashing;
}

bool PlayerData::CanAddAttackToBuffer()
{
	return CurrentPossessState == PlayerPossessState::None;
}

bool PlayerData::CanAttack()
{
	return CurrentAttackState == PlayerAttackState::None
	&& CurrentPossessState == PlayerPossessState::None;
}

bool PlayerData::CanDash()
{
	return !IsDashing
	&& CurrentPossessState == PlayerPossessState::None;
}

bool PlayerData::CanGroundSlam()
{
	return !IsDashing
	&& CurrentPossessState == PlayerPossessState::None;
}

bool PlayerData::CanEnterPossessMode()
{
	return true;
}

bool PlayerData::CanUsePossess()
{
	return CurrentPossessState == PlayerPossessState::PossessAim
	&& !IsDashing
	&& CurrentAttackState == PlayerAttackState::None;
}
