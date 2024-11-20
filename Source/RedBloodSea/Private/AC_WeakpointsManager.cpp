// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_WeakpointsManager.h"

#include "ComponentUtils.h"
#include "Engine/SimpleConstructionScript.h"

// Sets default values for this component's properties
UAC_WeakpointsManager::UAC_WeakpointsManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

// Called when the game starts
void UAC_WeakpointsManager::BeginPlay()
{
	Super::BeginPlay();

	owner = GetOwner();
	CreateWeakPoints();
}


// Called every frame
void UAC_WeakpointsManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

TArray<FName> UAC_WeakpointsManager::GetAllWeakpointsSockets()
{
	TArray<FName> result;
	if(skeleton == nullptr)
		return result;
	 for (auto name : skeleton->GetAllSocketNames())
	 {
	 	if(name.ToString().StartsWith("wp_"))
	 	{
	 		result.Add(name);
	 	}
	 }
	return result;
}

void UAC_WeakpointsManager::CreateWeakPoints()
{
	owner = GetOwner();
	skeleton = Cast<USkeletalMeshComponent>(owner->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	int count = 0;
	for (auto const & WeakPoint : WeakpointsSockets)
	{
		if(WeakPoint.bIsUsed)
			continue;
		switch (WeakPoint.Type)
		{
		case EWeakpointType::WPE_1:
			count += 10;
			break;
		case EWeakpointType::WPE_2:
			count += 5;
			break;
		case EWeakpointType::WPE_3:
			count += 1;
			break;
		}
	}
	
	for (int i = 0; i < Nb_weakpoints; i++)
	{
		int randomInt = FMath::RandRange(0, count - 1);
		for (auto & weakpoint : WeakpointsSockets)
		{
			if(weakpoint.bIsUsed)
				continue;
		
			switch (weakpoint.Type)
			{
			case EWeakpointType::WPE_1:
				randomInt -= 10;
				break;
			case EWeakpointType::WPE_2:
				randomInt -= 5;
				break;
			case EWeakpointType::WPE_3:
				randomInt -= 1;
				break;
			}
			if(randomInt < 0)
			{
				AttachWeakpoint(weakpoint.SocketName,weakpoint.MaxOffset);
				weakpoint.bIsUsed = true;
				switch (weakpoint.Type)
				{
				case EWeakpointType::WPE_1:
					count -= 10;
					break;
				case EWeakpointType::WPE_2:
					count -= 5;
					break;
				case EWeakpointType::WPE_3:
					count -= 1;
					break;
				}
				break;
			}
		}
	}
	
}


void UAC_WeakpointsManager::AttachWeakpoint(const FName& socketName,const UE::Math::TVector<double>& maxOffset )
{
	AActor* newActor = GetWorld()->SpawnActor(Weakpoint_BP);
	newActor->AttachToComponent(skeleton,FAttachmentTransformRules::SnapToTargetNotIncludingScale,socketName);
	UE::Math::TVector<double> offset = {
		FMath::FRandRange(-maxOffset.X,maxOffset.X),
		FMath::FRandRange(-maxOffset.Y,maxOffset.Y),
		FMath::FRandRange(-maxOffset.Z,maxOffset.Z)};
	newActor->SetActorRelativeLocation(offset);
}


	


