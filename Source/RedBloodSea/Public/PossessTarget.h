// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PossessTarget.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPossessed);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLinked, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPrepareForPossess);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUnpossessed, FVector, RepositionEnemyAtPosition);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class REDBLOODSEA_API UPossessTarget : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPossessTarget();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Possess();
	void Unpossess(FVector repositionEnemyAtPosition);
	void Link();
	void OnKilled();
	void PrepareForPossess();

	UPROPERTY(BlueprintAssignable, Category = "Possess")
	FOnPossessed OnPossessed;
	UPROPERTY(BlueprintAssignable, Category = "Possess")
	FOnUnpossessed OnUnpossessed;
	UPROPERTY(BlueprintAssignable, Category = "Possess")
	FOnPrepareForPossess OnPrepareForPossess;
	UPROPERTY(BlueprintAssignable, Category = "Possess")
	FOnLinked OnLinked;

	/*The duration, in seconds, the player has to wait before having control of the character again after camera zoom. This corresponds to the sword pull out animation*/
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = "Ground Slam")
	bool IsBearerBody = false;
};
