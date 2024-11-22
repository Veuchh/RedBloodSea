// Fill out your copyright notice in the Description page of Project Settings.


#include "ACWeakpointsManager.h"

#include "ComponentUtils.h"
#include "Engine/SimpleConstructionScript.h"

// Sets default values for this component's properties
UACWeakpointsManager::UACWeakpointsManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UACWeakpointsManager::BeginPlay()
{
	Super::BeginPlay();

	owner = GetOwner();
	skeleton = Cast<USkeletalMeshComponent>(owner->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	CreateWeakPoints();
}


// Called every frame
void UACWeakpointsManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	for (auto material : materialInstances)
	{
		UE::Math::TVector<double> position = weakpoints[0]->GetTransform().GetLocation();
		material->SetVectorParameterValue(TEXT("WeakPointPos1"),FLinearColor(position[0],position[1],position[2],0));
		material->SetScalarParameterValue(TEXT("Size"),10);
		material->SetScalarParameterValue(TEXT("TexSize"),2);
	}
	// ...
}

TArray<FName> UACWeakpointsManager::GetAllWeakpointsSockets()
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

void UACWeakpointsManager::SetSkeleton(USkeletalMeshComponent* newSkeleton)
{
	this->skeleton = newSkeleton;
}

void UACWeakpointsManager::SetMaterials(TArray<TObjectPtr<UMaterialInstanceDynamic>> newMaterialInstances)
{
	materialInstances = newMaterialInstances;
}

void UACWeakpointsManager::CreateWeakPoints()
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


void UACWeakpointsManager::AttachWeakpoint(const FName& socketName,const UE::Math::TVector<double>& maxOffset )
{
	AActor* newActor = GetWorld()->SpawnActor(Weakpoint_BP);
	newActor->AttachToComponent(skeleton,FAttachmentTransformRules::SnapToTargetNotIncludingScale,socketName);
	
	UE::Math::TVector<double> offset = {
		FMath::FRandRange(-maxOffset.X,maxOffset.X),
		FMath::FRandRange(-maxOffset.Y,maxOffset.Y),
		FMath::FRandRange(-maxOffset.Z,maxOffset.Z)};
	newActor->SetActorRelativeLocation(offset);
	
	weakpoints.Add(newActor);

	for (auto material : materialInstances)
	{
		UE::Math::TVector<double> position = newActor->GetTransform().GetLocation();
		material->SetVectorParameterValue(TEXT("WeakPointPos1"),FLinearColor(position[0],position[1],position[2],0));
		material->SetScalarParameterValue(TEXT("Size"),10);
		material->SetScalarParameterValue(TEXT("TexSize"),2);
	}
}


	


