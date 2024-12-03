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

	// You can expose some of your collision query data as properties to help customize and debug 
	// Here we expose the collision channel we want to run the query on, and set it to only hit Pawns.
	UPROPERTY(EditAnywhere, Category="Collision")
	TEnumAsByte<ECollisionChannel> TraceChannelProperty = ECC_Pawn;
	
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
