#pragma once

UENUM(BlueprintType)
enum class BufferableAttack : uint8 {
	None = 0 UMETA(DisplayName = "None"),
	Slash = 1 UMETA(DisplayName = "Slash"),
	Thrust = 2  UMETA(DisplayName = "Thrust")
};