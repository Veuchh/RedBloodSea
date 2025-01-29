// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "Weakpoint.h"
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
	void SetupPlayerCameraComponent(ACharacter* PlayerCharacter, UCameraComponent* PlayerCameraComponent,
	                                USceneComponent* newCameraRollTarget);
	void OnLookInput(FVector2D newLookInput);

private:
	void CameraRoll();
	void CameraFOV();
	void AimAssist(float deltaTime);
	AWeakpoint* GetAimAssistTarget();

	APlayerController* playerController;
	ACharacter* playerCharacter;
	UCameraComponent* playerCameraComponent;
	USceneComponent* cameraRollTarget;

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

	/*The target FOV when the player camera is moving to possess an ennemy*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Camera")
	float possessFOV = 80;

	/*How fast the FOV will change*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Camera")
	float fovChangeSpeed = 10;

	/*The max distance a weakpoint has to be from the camera in order for the aim assist to kick in*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "AimAssist")
	float aimAssistMaxDistanceFromWeakpoint = 250;

	/*The aim assist strength, or speed at wich it goes toward the target*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "AimAssist")
	float aimAssistStrength = 60;

	/*The max distance of the weakpoint on the screen center before the aim assist kicks in*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "AimAssist")
	float aimAssistMaxDistanceFromScreenCenter = .2f;

	bool isCameraAttachedToPlayer = false;

	float ToggleCameraAttachToPlayer;
};
