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
	ASpaceGladiatorPlayerController(const FObjectInitializer &ObjectInitializer);
	void BeginPlayingState() override;
	void SetupInputComponent() override;
	void TurnLeft();
	void TurnRight();
	void RotateCamera(FRotator Rotation);
	void RotateTargeting(FRotator Rotation);
	void Turn(float Value);
	void TiltCamera(float Value);
	virtual void PlayerTick(float DeltaTime) override;
	void FireHold();
	void Fire();
	void Recall();
	void PlaceWall();
	bool IsPlacingWalls();
	void UseItem();
	bool GetIsControlInverted();
};
