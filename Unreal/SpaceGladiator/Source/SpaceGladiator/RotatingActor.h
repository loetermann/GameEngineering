// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "RotatingActor.generated.h"

UCLASS()
class SPACEGLADIATOR_API ARotatingActor : public AActor
{
	GENERATED_BODY()

	float YawPerSeconds = 40.0f;
public:	
	// Sets default values for this actor's properties
	ARotatingActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void RotateInPlace(float value);
	
};
