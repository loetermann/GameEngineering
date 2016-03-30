// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "SGAIController.generated.h"

/**
 * 
 */
UCLASS()
class SPACEGLADIATOR_API ASGAIController : public AAIController
{
	GENERATED_BODY()
	
	ASGAIController(const FObjectInitializer &ObjectInitializer);

	void BeginPlay() override;
	
};
