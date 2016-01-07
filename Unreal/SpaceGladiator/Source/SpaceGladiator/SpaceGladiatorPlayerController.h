// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "WallSegment.h"
#include "SpaceGladiatorPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SPACEGLADIATOR_API ASpaceGladiatorPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
public:
	void BeginPlayingState() override;
	void SetupInputComponent() override;
	void TurnLeft();
	void TurnRight();
	void RotateCamera(FRotator Rotation);
	void RotateTargeting(FRotator Rotation);
	void Turn(float Value);
	void TiltCamera(float Value);
	void PlayerTick(float DeltaTime);
	void Fire();
	void Recall();
	

	AWallSegment *CurrentWall;
	void PlaceWall();
	void AddWallSegment();
};
