// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ACWeakpointsManager.h"
#include "GameFramework/Character.h"
#include "ADweller.generated.h"

UCLASS()
class REDBLOODSEA_API AADweller : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AADweller();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UACWeakpointsManager> WeakpointsManager;
	TArray<TObjectPtr<UMaterialInstanceDynamic>> MaterialInstances;
	
protected:
	// Called when the game starts or when spawned
	void OnConstruction(const FTransform &Transform) override;
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
