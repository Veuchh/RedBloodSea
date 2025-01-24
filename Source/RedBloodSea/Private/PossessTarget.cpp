// Fill out your copyright notice in the Description page of Project Settings.


#include "PossessTarget.h"

#include "DwellerLinkSubsystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UPossessTarget::UPossessTarget()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPossessTarget::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UPossessTarget::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UPossessTarget::Possess()
{
	OnPossessed.Broadcast();
}

void UPossessTarget::Unpossess(FVector repositionEnemyAtPosition)
{
	OnUnpossessed.Broadcast(repositionEnemyAtPosition);
}

void UPossessTarget::Link()
{
	OnLinked.Broadcast(GetOwner());
}

void UPossessTarget::OnKilled()
{
	UGameplayStatics::GetGameInstance(GetWorld())->GetSubsystem<UDwellerLinkSubsystem>()->OnDwellerKilled(this);
}

void UPossessTarget::PrepareForPossess()
{
	OnPrepareForPossess.Broadcast();
}

