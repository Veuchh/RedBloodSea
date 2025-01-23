// Fill out your copyright notice in the Description page of Project Settings.


#include "Weakpoint.h"

#include "Components/BoxComponent.h"

// Sets default values
AWeakpoint::AWeakpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	Collider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Collider);
	State = EWeakpointState::Hidden;
	
	//Mesh->SetupAttachment(Collider);
	//mesh->SetMaterial(0,)

}

// Called when the game starts or when spawned
void AWeakpoint::BeginPlay()
{
	Super::BeginPlay();
	Mesh->SetVisibility(false);
	if(!WeakpointsMeshes.IsEmpty())
		Mesh->SetStaticMesh(WeakpointsMeshes[FMath::RandRange(0,WeakpointsMeshes.Num()-1)]);

}

// Called every frame
void AWeakpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TObjectPtr<UStaticMeshComponent> AWeakpoint::GetMesh()
{
	return Mesh;
}

TObjectPtr<USphereComponent> AWeakpoint::GetCollider()
{
	return Collider;
}


