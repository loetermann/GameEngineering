// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <vector>
#include "GameFramework/GameMode.h"
#include "SGHUD.h"
#include "SpaceGladiatorGameMode.generated.h"
/**
 * 
 */
UCLASS()
class SPACEGLADIATOR_API ASpaceGladiatorGameMode : public AGameMode
{
	GENERATED_BODY()


	std::vector<FLinearColor> colors;

	int32 id;

public:
	ASpaceGladiatorGameMode(const FObjectInitializer &ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "SpaceGladiatorGameMode")
	FLinearColor getColor();

	UFUNCTION(BlueprintCallable, Category = "SpaceGladiatorGameMode")
		int32 nextID();
};
