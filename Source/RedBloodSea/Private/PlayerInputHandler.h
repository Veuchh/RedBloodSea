// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "PlayerMovement.h"
#include "PlayerCombat.h"
#include "PlayerCameraHandler.h"
#include "PlayerPossess.h"
#include "Components/ActorComponent.h"                    
#include "PlayerInputHandler.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPlayerInputHandler : public UActorComponent
{
	GENERATED_BODY()

private:
	UPlayerMovement* playerMovement;
	UPlayerCombat* playerCombat;
	UPlayerCameraHandler* playerCameraHandler;
	UPlayerPossess* playerPossess;
	
	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Dash Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* DashAction;

	/** Slash Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* GroundSlamAction;

	/** Slash Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SlashAction;

	/** Thrust & possess Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* ThrustAction;

	/** Possess mode Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PossessModeAction;

	void MoveInput(const FInputActionValue& Value);
	void LookInput(const FInputActionValue& Value);
	void JumpInput(const FInputActionValue& Value);
	void StopJumpInput(const FInputActionValue& Value);
	void DashInput(const FInputActionValue& Value);
	void GroundSlamInput(const FInputActionValue& Value);
	void SlashInput(const FInputActionValue& Value);
	void ThrustInput(const FInputActionValue& Value);
	void PossessInput(const FInputActionValue& Value);
	void PossessModeInput(const FInputActionValue& Value);

public:	
	// Sets default values for this component's properties
	UPlayerInputHandler();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetupPlayerInputComponent(UInputComponent* InputComponent);
};
