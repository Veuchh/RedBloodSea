// Fill out your copyright notice in the Description page of Project Settings.


#include "WeakpointsManager.h"
#include "UDataSubsystem.h"
#include "PossessTarget.h"

TArray<AWeakpoint*> UWeakpointsManager::GlobalWeakpointList = TArray<AWeakpoint*>();

// Sets default values for this component's properties
UWeakpointsManager::UWeakpointsManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Weakpoints.Reserve(10);
}

// Called when the game starts
void UWeakpointsManager::BeginPlay()
{
	Super::BeginPlay();

	Owner = GetOwner();
	Skeleton = Cast<USkeletalMeshComponent>(Owner->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	CreateWeakPoints();
	CountSubSys = Cast<UUDataSubsystem>(GetWorld()->GetGameInstance()->GetSubsystem<UUDataSubsystem>());
}


// Called every frame
void UWeakpointsManager::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	int index = 1;
	for (auto weakpoint : Weakpoints)
	{
		for (auto material : MaterialInstances)
		{
			UE::Math::TVector<double> position = weakpoint.Value->GetTransform().GetLocation();
			FName name = *FString("WeakPointPos").Append(FString::FromInt(index));
			material->SetVectorParameterValue(name, FLinearColor(position[0], position[1], position[2], 0));
		}
		index++;
	}
	// ...
}

void UWeakpointsManager::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	for (auto Element : Weakpoints)
	{
		if (GlobalWeakpointList.Contains(Element.Value))
		{
			GlobalWeakpointList.Remove(Element.Value);
		}
	}
	
	Super::OnComponentDestroyed(bDestroyingHierarchy);
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
	if (!IsValid(WeakpointData))
		return;

	int count = 0;
	for (auto const& WeakpointSlot : WeakpointData->WeakpointsSockets)
	{
		if (Weakpoints.Contains(WeakpointSlot.SocketName) || !(TypeFilter & static_cast<uint8>(WeakpointSlot.Type)))
			continue;
		count += WeakpointData->WeightTable[static_cast<int>(WeakpointSlot.Weight)];
	}

	int Size = static_cast<int>(EWeakpointSize::BIG);
	for (auto SizeCount : SizeNumber)
	{
		for (int i = 0; i < SizeCount; i++)
		{
			int randomInt = FMath::RandRange(0, count - 1);
			for (auto& WeakpointSlot : WeakpointData->WeakpointsSockets)
			{
				if (Weakpoints.Contains(WeakpointSlot.SocketName) || !(TypeFilter & static_cast<uint8>(WeakpointSlot.
					Type)))
					continue;

				randomInt -= WeakpointData->WeightTable[static_cast<int>(WeakpointSlot.Weight)];

				if (randomInt < 0)
				{
					AttachWeakpoint(WeakpointSlot, WeakpointData->SizeChart[Size]);
					count -= WeakpointData->WeightTable[static_cast<int>(WeakpointSlot.Weight)];
					break;
				}
			}
		}
		Size += 1;
	}
}


void UWeakpointsManager::AttachWeakpoint(const FWeakpointSlot& WeakpointSlot, const float Size)
{
	MaxHealthPoint++;
	HealthPoint++;
	AActor* newActor = GetWorld()->SpawnActor(WeakpointData->Weakpoints_BP);
	newActor->AttachToComponent(Skeleton, FAttachmentTransformRules::SnapToTargetNotIncludingScale,
	                            WeakpointSlot.SocketName);

	UE::Math::TVector<double> offset = {
		FMath::FRandRange(-WeakpointSlot.MaxOffset.X, WeakpointSlot.MaxOffset.X),
		FMath::FRandRange(-WeakpointSlot.MaxOffset.Y, WeakpointSlot.MaxOffset.Y),
		FMath::FRandRange(-WeakpointSlot.MaxOffset.Z, WeakpointSlot.MaxOffset.Z)
	};
	newActor->SetActorRelativeLocation(offset);

	const FRotator rot = {
		FMath::FRand()*360.f,
		FMath::FRand()*360.f,
		FMath::FRand()*360.f
	};
	newActor->SetActorRotation(rot);
	
	TObjectPtr<AWeakpoint> Weakpoint = Cast<AWeakpoint>(newActor);
	Weakpoint->SetActorScale3D(FVector(1, 1, 1) * Size);
	Weakpoints.Add(WeakpointSlot.SocketName, Weakpoint);
	//UsedWeakpointsSocketsNames.AddUnique(WeakpointSlot.SocketName); //TODO Find why this make the game crash in build and not in editor
	int index = Weakpoints.Num();
	for (auto material : MaterialInstances)
	{
		UE::Math::TVector<double> position = newActor->GetTransform().GetLocation();
		FName name = *FString("WeakPointPos").Append(FString::FromInt(index));
		FName size = *FString("Size").Append(FString::FromInt(index));
		material->SetVectorParameterValue(name, FLinearColor(position[0], position[1], position[2], 0));
		material->SetScalarParameterValue(size, 60 * Size);
	}

	GlobalWeakpointList.Add(Weakpoint);
}

void UWeakpointsManager::RevealWeakpoints()
{
	int index = 0;
	for (auto weakpoint : Weakpoints)
	{
		index++;

		if (weakpoint.Value->State != EWeakpointState::Hidden)
			continue;

		weakpoint.Value->GetMesh()->SetVisibility(true);
		weakpoint.Value->State = EWeakpointState::Revealed;
		for (auto material : MaterialInstances)
		{
			UE::Math::TVector<double> position = weakpoint.Value->GetTransform().GetLocation();
			FName name = *FString("Opacity").Append(FString::FromInt(index));
			material->SetScalarParameterValue(name, 1.0);
		}
	}
	OnWeakpointReveal.Broadcast();
	GetWorld()->GetTimerManager().SetTimer(WeakpointsVisibilityWindowTimer, this, &UWeakpointsManager::HideWeakpoints,
	                                       WeakpointData->WeakpointsVisibilityWindowLength, false);
}

void UWeakpointsManager::HideWeakpoints()
{
	// if(GEngine)
	// 	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Hiding Weakpoints"));
	int index = 0;
	for (auto weakpoint : Weakpoints)
	{
		index++;

		if (weakpoint.Value->State == EWeakpointState::Revealed)
			weakpoint.Value->State = EWeakpointState::Hidden;
		else if (weakpoint.Value->State == EWeakpointState::Damaged)
			continue;

		weakpoint.Value->GetMesh()->SetVisibility(false);

		for (auto material : MaterialInstances)
		{
			FName name = *FString("Opacity").Append(FString::FromInt(index));
			material->SetScalarParameterValue(name, 0.0);
		}
	}
	OnWeakpointHide.Broadcast();
}

void UWeakpointsManager::RemoveWeakpoint(AWeakpoint* weakpoint, bool canDestroyHiddenWeakpoints)
{
	TArray<TObjectPtr<AWeakpoint>> WeakpointsActors;
	Weakpoints.GenerateValueArray(WeakpointsActors);
	if (!WeakpointsActors.Contains(weakpoint)
		|| (!canDestroyHiddenWeakpoints && weakpoint->State != EWeakpointState::Revealed)
		|| (canDestroyHiddenWeakpoints && weakpoint->State == EWeakpointState::Damaged))
		return;
	HealthPoint--;
	int index = WeakpointsActors.IndexOfByKey(weakpoint) + 1;
	weakpoint->GetMesh()->SetVisibility(false);
	weakpoint->GetCollider()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	weakpoint->State = EWeakpointState::Damaged;
	
	if (GlobalWeakpointList.Contains(weakpoint))
	{
		GlobalWeakpointList.Remove(weakpoint);
	}
	
	for (auto material : MaterialInstances)
	{
		FName name = *FString("Opacity").Append(FString::FromInt(index));
		material->SetScalarParameterValue(name, 0.0);
	}
	OnWeakpointHit.Broadcast(weakpoint);
	if (CheckIfDead())
	{
		UPossessTarget* possessTarget = GetOwner()->GetComponentByClass<UPossessTarget>();

		if (possessTarget)
		{
			possessTarget->OnKilled();
		}
		OnDeath.Broadcast(Owner);
		CountSubSys->OnKill.Broadcast();
		//Owner->Destroy();
	}
}

void UWeakpointsManager::ClearAllWeakpoints()
{
	for (auto Weakpoint : Weakpoints)
	{
		if (GlobalWeakpointList.Contains(Weakpoint.Value))
		{
			GlobalWeakpointList.Remove(Weakpoint.Value);
		}
		
		Weakpoint.Value->Destroy();
	}
	Weakpoints.Empty();
}

AWeakpoint* UWeakpointsManager::GetRandomAliveWeakPoint()
{
	for (auto Weakpoint : Weakpoints)
	{
		if (Weakpoint.Value->State != EWeakpointState::Damaged)
			return Weakpoint.Value;
	}
	return nullptr;
}
