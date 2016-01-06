// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "RotatingActor.h"


// Sets default values
ARotatingActor::ARotatingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARotatingActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARotatingActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	RotateInPlace(DeltaTime);
}

void ARotatingActor::RotateInPlace(float value)
{
	FRotator rotation = GetActorRotation();
	rotation.Yaw += YawPerSeconds * value;
	SetActorRotation(rotation);
}
