#pragma once

#include "CoreMinimal.h"
#include "PlayerData.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "PlayerPossess.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnThrowRapierHitNothing);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
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
	
	float nextAllowedAction = 0;
public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Sets default values for this component's properties
	UPlayerPossess();
	void SetupPlayerPossessComponent(ACharacter* RedBloodSeaCharacter, UCameraComponent* CameraComponent);
	void OnPossessModeInput(bool isToggled);
	void OnPossessInput();

	UPROPERTY(BlueprintAssignable, Category = "Possess")
	FOnThrowRapierHitNothing OnThrowRapierHitNothing;
};
