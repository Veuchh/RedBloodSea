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
	UPossessTarget* currentlyPossessedDweller = dwellersInLink[dwellersInLink.Num() - 1];
	dwellersInLink.Empty();
	dwellersInLink.Add(currentlyPossessedDweller);
}

void UDwellerLinkSubsystem::UpdateLinkGFX(FVector currentPlayerPosition)
{
	for (int i = 0; i < dwellersInLink.Num() - 2; i++)
	{
		// if (dwellersInLink[i + 1] == nullptr)
		// {
		// 	dwellersInLink.RemoveAt(i + 1);
		// 	continue;
		// }
		FVector startPos = dwellersInLink[i]->GetOwner()->GetActorLocation();
		FVector endPos = dwellersInLink[i + 1]->GetOwner()->GetActorLocation();;
		DrawDebugLine(GetWorld(), startPos, endPos, FColor::Yellow);
	}
	if (dwellersInLink.Num() > 1)
	{
		DrawDebugLine(GetWorld(), dwellersInLink[dwellersInLink.Num() - 2]->GetOwner()->GetActorLocation(),
		              currentPlayerPosition, FColor::Yellow);
	}
}

void UDwellerLinkSubsystem::AddDwellerToLink(UPossessTarget* dweller)
{
	bool resetListAtTheEnd = false;
	if (dwellersInLink.Contains(dweller))
	{
		//This is the case where we possess the last possessed ennemy
		if (dwellersInLink.Num() > 1 && dwellersInLink[dwellersInLink.Num() - 2] == dweller)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Should break link");

			resetListAtTheEnd = true;
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Should initiate link");

			//Linking all dwellers
			for (int i = dwellersInLink.Num() - 1; i >= 0; i--)
			{
				//Except the one we are currently possessing
				if (dwellersInLink[i] == dweller)
				{
					break;
				}
				
				dwellersInLink[i]->Link();
			}

			resetListAtTheEnd = true;
		}
	}

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Adding dweller to link list");
	dwellersInLink.Add(dweller);
	
	if (resetListAtTheEnd)
	{
		ResetLink();
	}
}

void UDwellerLinkSubsystem::OnDwellerKilled(UPossessTarget* killedDweller)
{
	//If dweller is in list, break link
	if (dwellersInLink.Contains(killedDweller))
	{
		ResetLink();
	}
}
