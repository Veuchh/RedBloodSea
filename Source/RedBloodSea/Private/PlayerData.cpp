#include "PlayerData.h"

FVector2D PlayerData::CurrentMovementInput = FVector2D::Zero();
bool PlayerData::IsDashing = false;
float PlayerData::DashEndTime = 0;
float PlayerData::NextAllowedDash = 0;

PlayerData::PlayerData()
{
}

PlayerData::~PlayerData()
{
}

bool PlayerData::CanMove()
{
	return !IsDashing;
}

bool PlayerData::CanAttack()
{
	return !IsDashing;
}

bool PlayerData::CanDash()
{
	return !IsDashing;
}

bool PlayerData::CanGroundSlam()
{
	return !IsDashing;
}
