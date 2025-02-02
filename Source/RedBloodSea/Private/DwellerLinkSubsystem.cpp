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
	if(!dwellersInLink.IsEmpty())
		UPossessTarget* currentlyPossessedDweller = dwellersInLink[dwellersInLink.Num() - 1];
	dwellersInLink.Empty();

	for (int i = instantiatedLinksVFX.Num() - 1; i >= 0; i--)
	{
		instantiatedLinksVFX[i]->Destroy();
	}
	instantiatedLinksVFX.Empty();
	ALinkSplineVFX::ResetID();
}


int UDwellerLinkSubsystem::AddDwellerToLink(UPossessTarget* dweller)
{
	int linkedEnemies = 0;
	if (dwellersInLink.Contains(dweller))
	{
		//This is the case where we possess the last possessed ennemy
		// if (dwellersInLink.Num() <= 1 || dwellersInLink[dwellersInLink.Num() - 2] != dweller)
		// {
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Should initiate link");

			//Linking all dwellers
			int index = dwellersInLink.IndexOfByKey(dweller);
			if(dwellersInLink.Num() >= index+1)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Should initiate link");
				for (int i = index+1; i < dwellersInLink.Num(); i++)
				{
					//Except the one we are currently possessing
					if (dwellersInLink[i] == dweller)
					{
						break;
					}
					linkedEnemies++;
					dwellersInLink[i]->Link();
				}
			}
		// }

		ResetLink();
	}
	//Add link VFX
	else if (dwellersInLink.Num() >= 1)
	{
		if (instantiatedLinksVFX.Num() >= 1)
		{
			instantiatedLinksVFX[instantiatedLinksVFX.Num() - 1]->SetTarget2(dwellersInLink[dwellersInLink.Num() - 1]->GetOwner());
		}

		FVector Location = FVector::Zero();
		FRotator Rotation = FRotator::ZeroRotator;
		FActorSpawnParameters SpawnInfo;
		ALinkSplineVFX* newLink = GetWorld()->SpawnActor<ALinkSplineVFX>(splineLinkVFXReference, Location, Rotation, SpawnInfo);
		newLink->SetTarget1(dwellersInLink[dwellersInLink.Num() - 1]->GetOwner());
		newLink->SetTarget2(playerActor);

		instantiatedLinksVFX.Add(newLink);
	}

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Adding dweller to link list");
	dwellersInLink.Add(dweller);
	return linkedEnemies;
}

void UDwellerLinkSubsystem::OnDwellerKilled(UPossessTarget* killedDweller)
{
	//If dweller is in list, break link
	if (dwellersInLink.Contains(killedDweller))
	{
		ResetLink();
	}
}

void UDwellerLinkSubsystem::SetupSubsystem(AActor* currentPlayerActor, TSubclassOf<ALinkSplineVFX> splineLinkVFXBP)
{
	playerActor = currentPlayerActor;
	splineLinkVFXReference = splineLinkVFXBP;
}
