#pragma once

UENUM(BlueprintType)
enum class PlayerPossessState : uint8 {
	None = 0 UMETA(DisplayName = "None"),
	PossessAim = 1 UMETA(DisplayName = "PossessAim"),
	ThrowFail = 2  UMETA(DisplayName = "ThrowFail"),
	ThrowTarget = 3  UMETA(DisplayName = "ThrowTarget"),
	ZoomingCamera = 4  UMETA(DisplayName = "ZoomingCamera"),
};