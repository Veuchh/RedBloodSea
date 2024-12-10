// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <Perception/AISenseConfig_Sight.h>
#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "RedBloodSeaAIPerception.generated.h"

/**
 * 
 */
UCLASS()
class REDBLOODSEA_API URedBloodSeaAIPerception : public UAIPerceptionComponent
{
	GENERATED_BODY()

	static UAISenseConfig* GetPerceptionSenseConfig(AAIController* Controller, TSubclassOf<UAISense> SenseClass);

	UFUNCTION(BlueprintCallable)
		static bool SetSightRange(AAIController* Controller, float NewSightRange);
	
};
