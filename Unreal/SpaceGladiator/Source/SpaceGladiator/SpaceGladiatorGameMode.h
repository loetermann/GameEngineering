// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "GameFramework/GameMode.h"
#include "SpaceGladiatorGameMode.generated.h"
/**
 * 
 */
UCLASS()
class SPACEGLADIATOR_API ASpaceGladiatorGameMode : public AGameMode
{
	GENERATED_BODY()


	std::vector<FLinearColor> colors;

public:
	ASpaceGladiatorGameMode(const FObjectInitializer &ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "SpaceGladiatorGameMode")
	FLinearColor getColor();
	
};
