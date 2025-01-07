// Fill out your copyright notice in the Description page of Project Settings.


#include "WeakpointsManager.h"
#include "CountLevelInstanceSubsystem.h"


// Sets default values for this component's properties
UWeakpointsManager::UWeakpointsManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Weakpoints.Reserve(10);
	//UsedWeakpointsSocketsNames.Reserve(10);
}

// Called when the game starts
void UWeakpointsManager::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	Skeleton = Cast<USkeletalMeshComponent>(Owner->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	CreateWeakPoints();
	CountSubSys = Cast<UCountLevelInstanceSubsystem>(GetWorld()->GetSubsystem<UCountLevelInstanceSubsystem>());
	//RevealWeakpoints();
}


// Called every frame
void UWeakpointsManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	int index = 1;
	for (auto weakpoint : Weakpoints)
	{
		for (auto material : MaterialInstances)
		{
			UE::Math::TVector<double> position = weakpoint.Value->GetTransform().GetLocation();
			FName name = *FString("WeakPointPos").Append(FString::FromInt(index));
			material->SetVectorParameterValue(name,FLinearColor(position[0],position[1],position[2],0));
		}
		index++;
	}
	// ...
}


void UWeakpointsManager::SetSkeleton(USkeletalMeshComponent* newSkeleton)
{
	this->Skeleton = newSkeleton;
}

void UWeakpointsManager::SetMaterials(const TArray<TObjectPtr<UMaterialInstanceDynamic>>& newMaterialInstances)
{
	MaterialInstances = newMaterialInstances;
}

bool UWeakpointsManager::CheckIfDead()
{
	return HealthPoint <= 0;
}

int UWeakpointsManager::GetHealthPoint()
{
	return HealthPoint;
}

int UWeakpointsManager::GetMaxHealthPoint()
{
	return MaxHealthPoint;
}

void UWeakpointsManager::CreateWeakPoints()
{
	Owner = GetOwner();
	int count = 0;
	for (auto const & WeakpointSlot : WeakpointData->WeakpointsSockets)
	{
		if(Weakpoints.Contains(WeakpointSlot.SocketName)  || !(TypeFilter & static_cast<uint8>(WeakpointSlot.Type)))
			continue;
		count += WeakpointData->WeightTable[static_cast<int>(WeakpointSlot.Weight)];
	}

	int Size = static_cast<int>(EWeakpointSize::BIG);
	for (auto SizeCount : SizeNumber)
	{
		for (int i = 0; i < SizeCount; i++)
		{
			int randomInt = FMath::RandRange(0, count - 1);
			for (auto & WeakpointSlot : WeakpointData->WeakpointsSockets)
			{
				if(Weakpoints.Contains(WeakpointSlot.SocketName) || !(TypeFilter & static_cast<uint8>(WeakpointSlot.Type)))
					continue;
			
				randomInt -= WeakpointData->WeightTable[static_cast<int>(WeakpointSlot.Weight)];
				
				if(randomInt < 0)
				{
					AttachWeakpoint(WeakpointSlot,WeakpointData->SizeChart[Size]);
					count -= WeakpointData->WeightTable[static_cast<int>(WeakpointSlot.Weight)];
					break;
				}
			}
		}
		Size +=1;
	}
	
}


void UWeakpointsManager::AttachWeakpoint(const FWeakpointSlot& WeakpointSlot,const float Size)
{
	MaxHealthPoint++;
	HealthPoint++;
	AActor* newActor = GetWorld()->SpawnActor(WeakpointData->Weakpoint_BP);
	newActor->AttachToComponent(Skeleton,FAttachmentTransformRules::SnapToTargetNotIncludingScale,WeakpointSlot.SocketName);
	
	UE::Math::TVector<double> offset = {
		FMath::FRandRange(-WeakpointSlot.MaxOffset.X,WeakpointSlot.MaxOffset.X),
		FMath::FRandRange(-WeakpointSlot.MaxOffset.Y,WeakpointSlot.MaxOffset.Y),
		FMath::FRandRange(-WeakpointSlot.MaxOffset.Z,WeakpointSlot.MaxOffset.Z)};
	newActor->SetActorRelativeLocation(offset);
	TObjectPtr<AWeakpoint> Weakpoint = Cast<AWeakpoint>(newActor);
	Weakpoint->SetActorScale3D(FVector(1,1,1)*Size);
	Weakpoints.Add(WeakpointSlot.SocketName,Weakpoint);
	//UsedWeakpointsSocketsNames.AddUnique(WeakpointSlot.SocketName); //TODO Find why this make the game crash in build and not in editor
	int index = Weakpoints.Num();
	for (auto material : MaterialInstances)
	{
		UE::Math::TVector<double> position = newActor->GetTransform().GetLocation();
		FName name = *FString("WeakPointPos").Append(FString::FromInt(index));
		FName size = *FString("Size").Append(FString::FromInt(index));
		material->SetVectorParameterValue(name,FLinearColor(position[0],position[1],position[2],0));
		material->SetScalarParameterValue(size,60*Size);
	}
}

void UWeakpointsManager::RevealWeakpoints()
{
	int index = 1;
	for (auto weakpoint : Weakpoints)
	{
		if(weakpoint.Value->State != EWeakpointState::Hidden)
			continue;
		weakpoint.Value->GetMesh()->SetVisibility(true);
		weakpoint.Value->State = EWeakpointState::Revealed;
		for (auto material : MaterialInstances)
		{
			UE::Math::TVector<double> position = weakpoint.Value->GetTransform().GetLocation();
			FName name = *FString("Opacity").Append(FString::FromInt(index));
			material->SetScalarParameterValue(name,1.0);
		}
		index++;
	}
	OnWeakpointReveal.Broadcast();
}

void UWeakpointsManager::RemoveWeakpoint(AWeakpoint* weakpoint, bool canDestroyHiddenWeakpoints)
{
	TArray<TObjectPtr<AWeakpoint>> WeakpointsActors;
	Weakpoints.GenerateValueArray(WeakpointsActors);
	if(!WeakpointsActors.Contains(weakpoint)
		|| (!canDestroyHiddenWeakpoints && weakpoint->State != EWeakpointState::Revealed)
		|| (canDestroyHiddenWeakpoints &&  weakpoint->State == EWeakpointState::Damaged))
		return;
	HealthPoint--;
	int index = WeakpointsActors.IndexOfByKey(weakpoint)+1;
	weakpoint->GetMesh()->SetVisibility(false);
	weakpoint->GetCollider()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	weakpoint->State = EWeakpointState::Damaged;
	for (auto material : MaterialInstances)
	{
		FName name = *FString("Opacity").Append(FString::FromInt(index));
		material->SetScalarParameterValue(name,0);
	}
	OnWeakpointHit.Broadcast(weakpoint);
	if(CheckIfDead())
	{
		OnDeath.Broadcast(Owner);
		CountSubSys->OnKill.Broadcast();
		//Owner->Destroy();
	}
}

void UWeakpointsManager::ClearAllWeakpoints()
{
	for (auto Weakpoint : Weakpoints)
	{
		Weakpoint.Value->Destroy();
	}
	Weakpoints.Empty();
}

AWeakpoint* UWeakpointsManager::GetRandomAliveWeakPoint()
{
	for (auto Weakpoint : Weakpoints)
	{
		if(Weakpoint.Value->State != EWeakpointState::Damaged)
			return Weakpoint.Value;
	}
	return nullptr;
}



	


