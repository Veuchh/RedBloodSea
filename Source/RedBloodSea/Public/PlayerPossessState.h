#pragma once

UENUM(BlueprintType)
enum class PlayerPossessState : uint8 {
	None = 0 UMETA(DisplayName = "None"),
	TogglingAimMode = 1 UMETA(DisplayName = "TogglingAimMode"),
	PossessAim = 2 UMETA(DisplayName = "PossessAim"),
	ThrowFail = 3  UMETA(DisplayName = "ThrowFail"),
	ThrowTarget = 4  UMETA(DisplayName = "ThrowTarget"),
	ZoomingCamera = 5  UMETA(DisplayName = "ZoomingCamera"),
};