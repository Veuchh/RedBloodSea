// Fill out your copyright notice in the Description page of Project Settings.


#include "DwellerLinkSubsystem.h"

void UDwellerLinkSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "DwellerLinkSubsystem::Initialize");
	Super::Initialize(Collection);
}

void UDwellerLinkSubsystem::Deinitialize()
{
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "DwellerLinkSubsystem::DeInitialize");
	Super::Deinitialize();
}

void UDwellerLinkSubsystem::ResetLink()
{
}

void UDwellerLinkSubsystem::UpdateLinkGFX(FVector currentPlayerPosition)
{
	for (int i = 0; i < dwellersInLink.Num() - 2; i++)
	{
		FVector startPos = dwellersInLink[i]->GetOwner()->GetActorLocation();
		FVector endPos = dwellersInLink[i + 1]->GetOwner()->GetActorLocation();;
		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Yellow);
	}
	if (dwellersInLink.Num() > 2)
	{
		DrawDebugLine(GetWorld(), dwellersInLink[dwellersInLink.Num()-2]->GetOwner()->GetActorLocation(), currentPlayerPosition, FColor::Yellow);
	}
}

void UDwellerLinkSubsystem::AddDwellerToLink(UPossessTarget* dweller)
{
	dwellersInLink.Add(dweller);
	//Check if dweller is already in list
	//	If yes and Dweller is not in the last 2 members of the list, initiate link in the list starting from the already existing dweller
	//	else if Dweller is in the last 2 members of the list, clear the list and break the link
	//	else add to the list
}

void UDwellerLinkSubsystem::OnDwellerKilled(UPossessTarget* dweller)
{
	//If dweller is in list, break link
}
