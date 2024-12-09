#pragma once

UENUM(BlueprintType)
enum class PlayerPossessState : uint8 {
	None = 0 UMETA(DisplayName = "None"),
	TogglingAimMode = 1 UMETA(DisplayName = "TogglingAimMode"),
	PossessAim = 2 UMETA(DisplayName = "PossessAim"),
	ThrowFailNotPossessing = 3  UMETA(DisplayName = "ThrowFailNotPossessing"),
	ThrowTarget = 4  UMETA(DisplayName = "ThrowTarget"),
	ZoomingCamera = 5  UMETA(DisplayName = "ZoomingCamera"),
	PossessRecovery = 6  UMETA(DisplayName = "PossessRecovery"),
	ThrowFailWhilePossessing = 7  UMETA(DisplayName = "ThrowFailWhilePossessing"),
};