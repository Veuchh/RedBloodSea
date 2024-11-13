#pragma once

#include "CoreMinimal.h"
#include "BufferableAttack.h"

/**
 * 
 */
static class REDBLOODSEA_API PlayerData
{
public:
	PlayerData();
	~PlayerData();
	//-------------------------------------------Movement
	static FVector2D CurrentMovementInput;
	static bool IsDashing;
	static float DashEndTime;
	static float NextAllowedDash;
	static TArray<BufferableAttack> AttackBuffer;
	static float NextAllowedAttack;

	static bool CanMove();
	static bool CanAttack();
	static bool CanDash();
	static bool CanGroundSlam();
};
