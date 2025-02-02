#pragma once

#include "CoreMinimal.h"
#include "PlayerPossessState.h"
#include "BufferableAttack.h"
#include "PossessTarget.h"
#include "PlayerAttackState.h"

static class REDBLOODSEA_API PlayerData
{
public:
	PlayerData();
	~PlayerData();
	static void ResetData();

	//-------------------------------------------Movement
	static FVector2D CurrentMovementInput;
	static bool IsDashing;
	static int RemainingAirDashes;
	static float DashEndTime;
	static float NextAllowedDash;
	static float NextAllowedGroundSlam;
	
	//-------------------------------------------Combat
	static bool IsGodModeEnabled;
	static int MaxHPAmount;
	static int CurrentHPAmount;
	static float GetCurrentAttackStartupEndTime();
	static float GetCurrentAttackCooldownEndTime();
	static float GetCurrentAttackColliderEndTime();
	static BufferableAttack CurrentAttack;
	static TArray<BufferableAttack> AttackBuffer;
	static int MaxAttackBufferCapacity;
	static PlayerAttackState CurrentAttackState;
	static float AttackStartTime;
	static float LastHitTime;
	static float NextAllowedInputBufferTime;
	
	//-----------------------------------------------------------------------------------Slash
	static float SlashAttackStartupDelay;
	static float SlashAttackDuration;
	static float SlashAttackCooldown;

	//-----------------------------------------------------------------------------------Thrust
	static float ThrustAttackStartupDelay;
	static float ThrustAttackDuration;
	static float ThrustAttackCooldown;

	//-------------------------------------------Possession
	static PlayerPossessState CurrentPossessState;
	static float StartCameraMovementTime;
	static float EndCameraMovementTime;
	static UPossessTarget* CurrentPossessTarget;
	
	//-------------------------------------------Test methods for the player
	static bool CanMove();
	static bool CanJump();
	static bool CanRotateCamera();
	static bool CanAddAttackToBuffer();
	static bool CanAttack();
	static bool CanDash();
	static bool CanGroundSlam();
	static bool CanEnterPossessMode();
	static bool CanUsePossess();
};
