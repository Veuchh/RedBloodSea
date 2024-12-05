// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "PlayerCameraHandler.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class REDBLOODSEA_API UPlayerCameraHandler : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UPlayerCameraHandler();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void SetupPlayerCameraComponent(ACharacter* PlayerCharacter, UCameraComponent* PlayerCameraComponent);
	void OnLookInput(FVector2D newLookInput);

private:
	void CameraRoll();
	void CameraFOV();
	ACharacter* playerCharacter;
	UCameraComponent* playerCameraComponent;

	/*How much (in degrees) the camera will roll to the left or right when moving to the side*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Camera")
	float cameraRollStrength;

	/*How fast the camera will roll to the left or right when moving to the side*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Camera")
	float cameraRollSpeed;

	/*The default FOV*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Camera")
	float defaultFOV = 105;

	/*The target FOV when the player is dashing*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Camera")
	float dashFOV = 115;

	/*How fast the FOV will change*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Camera")
	float fovChangeSpeed = 10;

	bool isCameraAttachedToPlayer = false;

	float ToggleCameraAttachToPlayer;
};
