#pragma once

#include "CoreMinimal.h"
#include "PossessTarget.h"
#include "PlayerData.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "Dweller.h"
#include "PlayerPossess.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUpdateHPDisplay, int, newCurrentHP, int, newMaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPossessAimStart);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPossessAimStop);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPossessRecovery);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThrowRapierNothing, FVector, rapierEndPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThrowRapierTarget, FVector, rapierEndPosition);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnThrowRapierEnviro, FVector, rapierEndPosition);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class REDBLOODSEA_API UPlayerPossess : public UActorComponent
{
	GENERATED_BODY()

private:
	ACharacter* character;
	UCameraComponent* camera;

	/*The max distance of the possess mechanic*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Possess Timings")
	float maxPossessDistance = 1000;

	/*The duration, in seconds, the player has to hold E to be able to throw the rapier*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Possess Timings")
	float holdDelayToEnterAimingMode = .2;

	/*The duration, in seconds, the player has to wait before having control of the rapier again if it hits a wall or nothing*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Possess Timings")
	float throwFailDuration = 1;

	UPROPERTY(EditAnywhere, Category="Collision")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;

	/*The duration, in seconds, of the delay before the camera starts to zoom when possessing*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Possess Timings")
	float possessDelay = .2;

	/*The duration, in seconds, the camera travels during a possession*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Possess Timings")
	float possessTransitionDuration = .2;

	/*The duration, in seconds, the player has to wait before having control of the character again after camera zoom. This corresponds to the sword pull out animation*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Possess Timings")
	float playerControlDelay = .4;
	UPROPERTY(EditDefaultsOnly,Category="ActorSpawning")
	TSubclassOf<ADweller> dwellerBP;
	
	float nextAllowedAction = 0;
	bool isInputModeActionPressed = false;
	FVector startPossessPosition;
	FVector endPossessPosition;
	UPossessTarget* targetToUnpossess;

	void AimModeToggling();
	void TogglePlayer(bool isToggled) const;

	void UpdatePlayerHealth();
	void CameraZoomTick();
	void SetupCameraMovement();
	void ThrowTargetTick();
	void ThrowFailTick();
	void AimModeTogglingTick();
	void ThrowFailWhilePossessingTick();
	void PossessRecoveryTick();
	void LineTrace(FVector TraceStart, FVector TraceEnd, FHitResult& Hit);
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	// Sets default values for this component's properties
	UPlayerPossess();
	void SetupPlayerPossessComponent(ACharacter* RedBloodSeaCharacter, UCameraComponent* CameraComponent);
	void OnPossessModeInput(bool isToggled);
	void OnPossessInput();
	
	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnUpdateHPDisplay OnUpdateHPDisplay;

	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnPossessAimStart OnPossessAimStart;

	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnPossessAimStop OnPossessAimStop;

	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnPossessRecovery OnPossessRecovery;

	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnThrowRapierNothing OnThrowRapierNothing;

	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnThrowRapierTarget OnThrowRapierTarget;

	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FOnThrowRapierEnviro OnThrowRapierEnviro;
};
