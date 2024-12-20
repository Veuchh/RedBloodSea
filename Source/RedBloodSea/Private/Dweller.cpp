// Fill out your copyright notice in the Description page of Project Settings.


#include "Dweller.h"

// Sets default values
ADweller::ADweller()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeakpointsManager = CreateDefaultSubobject<UWeakpointsManager>(TEXT("WeakpointManager"));
	WeakpointsManager->SetSkeleton(GetMesh());
	
}

void ADweller::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	TArray<AActor*> ChildActors;
	GetAttachedActors(ChildActors,true);
	for (auto ChildActor : ChildActors)
	{
		ChildActor->Destroy();
	}
	Super::EndPlay(EndPlayReason);
}

UWeakpointsManager* ADweller::GetWeakpointManager()
{
	return WeakpointsManager;
}


// Called when the game starts or when spawned
void ADweller::BeginPlay()
{
	for (auto material : GetMesh()->GetMaterials())
	{
		MaterialInstances.Add(UMaterialInstanceDynamic::Create(material, this));
	}

	int index = 0;
	for (auto material : MaterialInstances)
	{
		GetMesh()->SetMaterial(index, material);
		index++;
	}
	WeakpointsManager->SetMaterials(MaterialInstances);
	
	Super::BeginPlay();
	
	
}

// Called every frame
void ADweller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADweller::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

