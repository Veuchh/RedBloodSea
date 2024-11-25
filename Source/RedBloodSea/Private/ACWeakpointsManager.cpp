// Fill out your copyright notice in the Description page of Project Settings.


#include "ACWeakpointsManager.h"

#include <string>

#include "ComponentUtils.h"
#include "DynamicMesh/DynamicMesh3.h"
#include "Engine/SimpleConstructionScript.h"

// Sets default values for this component's properties
UACWeakpointsManager::UACWeakpointsManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	WeightTable.Add(EWeakpointWeight::HIGH,10);
	WeightTable.Add(EWeakpointWeight::MID,5);
	WeightTable.Add(EWeakpointWeight::LOW,2);

	TypeFilter.Add(EWeakpointType::ARMS,true);
	TypeFilter.Add(EWeakpointType::LEGS,true);
	TypeFilter.Add(EWeakpointType::TORSO,true);
	TypeFilter.Add(EWeakpointType::HEAD,true);

	SizeChart.Add(EWeakpointSize::BIG,1);
	SizeChart.Add(EWeakpointSize::MEDIUM,1);
	SizeChart.Add(EWeakpointSize::SMALL,1);
}

// Called when the game starts
void UACWeakpointsManager::BeginPlay()
{
	Super::BeginPlay();

	owner = GetOwner();
	skeleton = Cast<USkeletalMeshComponent>(owner->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	CreateWeakPoints();
	//RevealWeakpoints();
}


// Called every frame
void UACWeakpointsManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	int index = 1;
	for (auto weakpoint : weakpoints)
	{
		for (auto material : materialInstances)
		{
			UE::Math::TVector<double> position = weakpoint->GetTransform().GetLocation();
			FName name = *FString("WeakPointPos").Append(FString::FromInt(index));
			material->SetVectorParameterValue(name,FLinearColor(position[0],position[1],position[2],0));
		}
		index++;
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
	int count = 0;
	for (auto const & WeakpointSlot : WeakpointsSockets)
	{
		if(WeakpointSlot.bIsUsed || !TypeFilter[WeakpointSlot.Type])
			continue;
		count += WeightTable[WeakpointSlot.Weight];
	}

	for (auto SizeCount : SizeNumber)
	{
		for (int i = 0; i < SizeCount.Value; i++)
		{
			int randomInt = FMath::RandRange(0, count - 1);
			for (auto & WeakpointSlot : WeakpointsSockets)
			{
				if(WeakpointSlot.bIsUsed || !TypeFilter[WeakpointSlot.Type])
					continue;
			
				randomInt -= WeightTable[WeakpointSlot.Weight];
				
				if(randomInt < 0)
				{
					AttachWeakpoint(WeakpointSlot,SizeChart[SizeCount.Key]);
					WeakpointSlot.bIsUsed = true;
					count -= WeightTable[WeakpointSlot.Weight];
					break;
				}
			}
		}
	}
	
}


void UACWeakpointsManager::AttachWeakpoint(const FWeakpointSlot& WeakpointSlot,const float Size)
{
	AActor* newActor = GetWorld()->SpawnActor(Weakpoint_BP);
	newActor->AttachToComponent(skeleton,FAttachmentTransformRules::SnapToTargetNotIncludingScale,WeakpointSlot.SocketName);
	
	UE::Math::TVector<double> offset = {
		FMath::FRandRange(-WeakpointSlot.MaxOffset.X,WeakpointSlot.MaxOffset.X),
		FMath::FRandRange(-WeakpointSlot.MaxOffset.Y,WeakpointSlot.MaxOffset.Y),
		FMath::FRandRange(-WeakpointSlot.MaxOffset.Z,WeakpointSlot.MaxOffset.Z)};
	newActor->SetActorRelativeLocation(offset);
	TObjectPtr<AWeakpoint> weakpoint = Cast<AWeakpoint>(newActor);
	weakpoint->SetActorScale3D(FVector(1,1,1)*Size);
	weakpoints.Add(weakpoint);
	weakpoint->OnActorBeginOverlap.AddDynamic(this,&UACWeakpointsManager::WeakpointOverlapBegin);
	int index = weakpoints.Num();
	for (auto material : materialInstances)
	{
		UE::Math::TVector<double> position = newActor->GetTransform().GetLocation();
		FName name = *FString("WeakPointPos").Append(FString::FromInt(index));
		FName size = *FString("Size").Append(FString::FromInt(index));
		material->SetVectorParameterValue(name,FLinearColor(position[0],position[1],position[2],0));
		material->SetScalarParameterValue(size,50*Size);
	}
}

void UACWeakpointsManager::RevealWeakpoints()
{
	int index = 1;
	for (auto weakpoint : weakpoints)
	{
		if(weakpoint->State != EWeakpointState::Hidden)
			continue;
		weakpoint->GetMesh()->SetVisibility(true);
		weakpoint->State = EWeakpointState::Revealed;
		for (auto material : materialInstances)
		{
			UE::Math::TVector<double> position = weakpoint->GetTransform().GetLocation();
			FName name = *FString("Opacity").Append(FString::FromInt(index));
			material->SetScalarParameterValue(name,1.0);
		}
		index++;
	}
}

void UACWeakpointsManager::RemoveWeakpoint(AWeakpoint* weakpoint)
{
	if(weakpoint->State != EWeakpointState::Revealed)
		return;
	int index = weakpoints.IndexOfByKey(weakpoint)+1;
	weakpoint->GetMesh()->SetVisibility(false);
	weakpoint->State = EWeakpointState::Damaged;
	for (auto material : materialInstances)
	{
		FName name = *FString("Opacity").Append(FString::FromInt(index));
		material->SetScalarParameterValue(name,0);
	}
}

void UACWeakpointsManager::WeakpointOverlapBegin(AActor* OverlapedActor, AActor* OtherActor)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red,OverlapedActor->GetName());
	if(weakpoints.Contains(OverlapedActor))
	{
		RemoveWeakpoint(Cast<AWeakpoint>(OverlapedActor));
	}
}


	


