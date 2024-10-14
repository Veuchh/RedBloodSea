// Copyright Epic Games, Inc. All Rights Reserved.

#include "RedBloodSeaGameMode.h"
#include "RedBloodSeaCharacter.h"
#include "UObject/ConstructorHelpers.h"

ARedBloodSeaGameMode::ARedBloodSeaGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}
