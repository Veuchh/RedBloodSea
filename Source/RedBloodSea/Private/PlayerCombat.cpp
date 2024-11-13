// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombat.h"

// Sets default values for this component's properties
UPlayerCombat::UPlayerCombat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UPlayerCombat::BeginPlay()
{
	Super::BeginPlay();

	USceneComponent* slashCollidersParent = static_cast<USceneComponent*>(slashCollidersParentReference.
		GetComponent(GetOwner()));

	// Check if the component is valid
	if (slashCollidersParent)
	{
		TArray<USceneComponent*> ChildComponents;
		slashCollidersParent->GetChildrenComponents(true, ChildComponents);
		// Get all child components of slashCollidersParent

		// Iterate through the child components
		for (USceneComponent* ChildComponent : ChildComponents)
		{
			UPrimitiveComponent* PrimitiveChild = Cast<UPrimitiveComponent>(ChildComponent);
			slashColliders.Add(PrimitiveChild);
			// Check if the child component is a primitive component (collider)
			if (PrimitiveChild)
			{
				PrimitiveChild->SetVisibility(false);
			}
		}
	}
}


// Called every frame
void UPlayerCombat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPlayerCombat::OnSlashInput()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Purple, "SLASH INPUT");
	for (UPrimitiveComponent* collider : slashColliders)
	{
		collider->SetVisibility(true);
	}
}

void UPlayerCombat::OnThrustInput()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "THRUST INPUT");
	for (UPrimitiveComponent* collider : slashColliders)
	{
		collider->SetVisibility(false);
	}
}

void UPlayerCombat::OnPossessInput()
{
}
