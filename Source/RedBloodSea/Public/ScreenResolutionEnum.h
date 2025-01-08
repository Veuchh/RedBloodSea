// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ScreenResolutionEnum.generated.h"

UENUM(BlueprintType)
enum class EScreenResolution : uint8
{
	Option1 UMETA(DisplayName = "1920*1080"),
	Option2 UMETA(DisplayName = "2560*1440"),
	Option3 UMETA(DisplayName = "1366*768"),
	Option4 UMETA(DisplayName = "1280*720"),
	Option5 UMETA(DisplayName = "1920*1200"),
	Option6 UMETA(DisplayName = "1680*1050"),
	Option7 UMETA(DisplayName = "1440*900"),
	Option8 UMETA(DisplayName = "1280*800"),
	Option9 UMETA(DisplayName = "1024*768"),
	Option10 UMETA(DisplayName = "800*600"),
	Option11 UMETA(DisplayName = "640*480"),
};
