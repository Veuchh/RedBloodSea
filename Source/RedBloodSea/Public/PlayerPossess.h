#pragma once

#include "CoreMinimal.h"
#include "PlayerData.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "PlayerPossess.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnThrowRapierHitNothing);

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
	
	float nextAllowedAction = 0;
	bool isInputModeActionPressed = false;
	FVector startPossessPosition;
	FVector endPossessPosition;

	void AimModeToggling();
	void TogglePlayer(bool isToggled) const;

public:
	void CameraZoomTick();
	void ThrowTargetTick();
	void ThrowFailTick();
	void AimModeTogglingTick();
	void PossessRecoveryTick();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	// Sets default values for this component's properties
	UPlayerPossess();
	void SetupPlayerPossessComponent(ACharacter* RedBloodSeaCharacter, UCameraComponent* CameraComponent);
	void OnPossessModeInput(bool isToggled);
	void OnPossessInput();

	UPROPERTY(BlueprintAssignable, Category = "Possess")
	FOnThrowRapierHitNothing OnThrowRapierHitNothing;
};
