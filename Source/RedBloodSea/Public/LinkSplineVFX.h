// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LinkSplineVFX.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateTarget1, AActor*, target1);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateTarget2, AActor*, target2);

UCLASS()
class REDBLOODSEA_API ALinkSplineVFX : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALinkSplineVFX();
	UPROPERTY(Category = "Targets", BlueprintReadWrite, EditAnywhere)
	AActor* Target1;
	UPROPERTY(Category = "Targets", BlueprintReadWrite, EditAnywhere)
	AActor* Target2;
	UPROPERTY(Category = "Audio", BlueprintReadWrite, EditAnywhere)
	int ThisSplineID;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	static int nextSplineID;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetTarget1(AActor* target);
	void SetTarget2(AActor* target);
	
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FUpdateTarget1 UpdateTarget1;
	UPROPERTY(BlueprintAssignable, Category = "Callbacks")
	FUpdateTarget2 UpdateTarget2;
};
