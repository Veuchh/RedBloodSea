// Fill out your copyright notice in the Description page of Project Settings.


#include "ADweller.h"

// Sets default values
AADweller::AADweller()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WeakpointsManager = CreateDefaultSubobject<UACWeakpointsManager>(TEXT("StaticMesh"));
	WeakpointsManager->SetSkeleton(GetMesh());
	
}

// void AADweller::OnConstruction(const FTransform &Transform)
// {
//
// }

// Called when the game starts or when spawned
void AADweller::BeginPlay()
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
void AADweller::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AADweller::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

