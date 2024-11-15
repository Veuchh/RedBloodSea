// Fill out your copyright notice in the Description page of Project Settings.


#include "AC_WeakpointsManager.h"

#include "ComponentUtils.h"

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

void UAC_WeakpointsManager::CreateWeakPoints()
{
	int WeakpointsSocketNb = 0;
	USkeletalMeshComponent* skeleton = Cast<USkeletalMeshComponent>(owner->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	TArray<FWeakPoint*> usedSockets;
	int count = 0;
	for (auto WeakPoint : WeakpointsSockets)
	{
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

	int randomInt = FMath::RandRange(0, count - 1);
	for (auto WeakPoint : WeakpointsSockets)
	{
		if(WeakPoint.bIsUsed)
			continue;
		
		switch (WeakPoint.Type)
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
			usedSockets.Add(&WeakPoint);
			WeakPoint.bIsUsed = true;
			switch (WeakPoint.Type)
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
	
	for (auto Element : usedSockets)
	{
		AActor* newActor = GetWorld()->SpawnActor(Weakpoint_BP);
		newActor->AttachToComponent(skeleton,FAttachmentTransformRules::SnapToTargetNotIncludingScale,Element->SocketName);
		UE::Math::TVector<double> offset = {
			FMath::FRandRange(-Element->MaxOffset.X,Element->MaxOffset.X),
			FMath::FRandRange(-Element->MaxOffset.Y,Element->MaxOffset.Y),
			FMath::FRandRange(-Element->MaxOffset.Z,Element->MaxOffset.Z)};
		newActor->SetActorRelativeLocation(offset);
	}
}

	


