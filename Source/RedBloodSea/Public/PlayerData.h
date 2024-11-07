#pragma once

#include "CoreMinimal.h"

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

	static bool CanMove();
};
