#pragma once

UENUM(BlueprintType)
enum class PlayerAttackState : uint8 {
	None = 0 UMETA(DisplayName = "None"),
	Startup = 1 UMETA(DisplayName = "Startup"),
	Attacking = 2  UMETA(DisplayName = "Attacking"),
	Cooldown = 3  UMETA(DisplayName = "Cooldown")
};