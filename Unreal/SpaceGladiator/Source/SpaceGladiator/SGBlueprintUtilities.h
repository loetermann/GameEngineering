// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "SGBlueprintUtilities.generated.h"

/**
 * 
 */
UCLASS()
class SPACEGLADIATOR_API USGBlueprintUtilities : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
	
public:
	UFUNCTION(BlueprintCallable, Category="SpaceGladiator")
	static FVector CalculateAIAiming(APawn *shooter, APawn *target, float projectileVelocity);
	
};
