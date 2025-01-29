// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Weakpoint.generated.h"

UENUM(BlueprintType)
enum class EWeakpointState : uint8 {
	Hidden		UMETA(DisplayName="Hidden"),
	Revealed    UMETA(DisplayName="Revealed"),
	Damaged     UMETA(DisplayName="Damaged"),
};

UCLASS()
class REDBLOODSEA_API AWeakpoint : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AWeakpoint();
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Weakpoints Handler")
	EWeakpointState State;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category="Weakpoints Handler")
	TArray<TObjectPtr<UStaticMesh>> WeakpointsMeshes;
private:
	TObjectPtr<UMaterialInstance> Material;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UStaticMeshComponent> Mesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USphereComponent> Collider;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	TObjectPtr<UStaticMeshComponent> GetMesh();
	TObjectPtr<USphereComponent>   GetCollider();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
