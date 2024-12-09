#pragma once

#include "CoreMinimal.h"
#include "PossessTarget.h"
#include "PlayerData.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "BearerBody.h"
#include "PlayerPossess.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class REDBLOODSEA_API UPlayerPossess : public UActorComponent
{
	GENERATED_BODY()

private:
	ACharacter* character;
	UCameraComponent* camera;

	/*The max distance of the possess mechanic*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Ground Slam")
	float maxPossessDistance = 1000;

	/*The duration, in seconds, the player has to hold E to be able to throw the rapier*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Ground Slam")
	float holdDelayToEnterAimingMode = .2;

	/*The duration, in seconds, the player has to wait before having control of the rapier again if it hits a wall or nothing*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Ground Slam")
	float throwFailDuration = 1;

	UPROPERTY(EditAnywhere, Category="Collision")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;

	/*The duration, in seconds, of the delay before the camera starts to zoom when possessing*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Ground Slam")
	float possessDelay = .2;

	/*The duration, in seconds, the camera travels during a possession*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Ground Slam")
	float possessTransitionDuration = .2;

	/*The duration, in seconds, the player has to wait before having control of the character again after camera zoom. This corresponds to the sword pull out animation*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Ground Slam")
	float playerControlDelay = .4;

	/*The duration, in seconds, the player waits before starting the camera transition when missing a throw while possessing a body*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Ground Slam")
	float throwFailWhilePossessingDelay = .2;

	/*Blueprint Reference of UsefulActor class*/
	UPROPERTY(EditDefaultsOnly,Category="ActorSpawning")
	TSubclassOf<ABearerBody> BearerBodyBP;
	
	float nextAllowedAction = 0;
	bool isInputModeActionPressed = false;
	FVector startPossessPosition;
	FVector endPossessPosition;
	ABearerBody* bearerBodyInstance;
	UPossessTarget* targetToUnpossess;

	void AimModeToggling();
	void TogglePlayer(bool isToggled) const;
	void LeaveBearerBodyAtPosition();

public:
	void CameraZoomTick();
	void SetupCameraMovement();
	void ThrowTargetTick();
	void ThrowFailTick();
	void AimModeTogglingTick();
	void ThrowFailWhilePossessingTick();
	void PossessRecoveryTick();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	// Sets default values for this component's properties
	UPlayerPossess();
	void SetupPlayerPossessComponent(ACharacter* RedBloodSeaCharacter, UCameraComponent* CameraComponent);
	void OnPossessModeInput(bool isToggled);
	void LineTrace(FVector TraceStart, FVector TraceEnd, FHitResult& Hit);
	void OnPossessInput();
};
