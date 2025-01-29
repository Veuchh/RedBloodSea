// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIStatesC.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EAIStatesC : uint8
{
	Aggressive,
	Afraid,
	Melee,
	Distance,
	Friendly,
	Hitstun,
	Dead,
	DebugIdle,
};