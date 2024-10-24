// Copyright Epic Games, Inc. All Rights Reserved.

#include "RedBloodSeaGameMode.h"
#include "RedBloodSeaCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARedBloodSeaGameMode::ARedBloodSeaGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/GameplayModules/Entities/Player/Blueprints/BP_FirstPersonCharacter.BP_FirstPersonCharacter_C"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
