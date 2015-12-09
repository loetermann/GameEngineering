// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "SpaceGladiatorPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SPACEGLADIATOR_API ASpaceGladiatorPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
	
public:
	void SetupInputComponent() override;
	void TurnLeft();
	void TurnRight();
	void Turn(float Value);
	void PlayerTick(float DeltaTime);
};
