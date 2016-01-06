// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "LaserProjectile.h"


// Sets default values
ALaserProjectile::ALaserProjectile(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Use a Capsule as a simple collision representation
	CollisionComp = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("CapsuleComp"));
	CollisionComp->InitCapsuleSize(5.0f, 20.0f);
	RootComponent = CollisionComp;
		
}

// Called when the game starts or when spawned
void ALaserProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(5);
}

// Called every frame
void ALaserProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	FHitResult HitResult;
	if (SetActorLocation(GetActorLocation()+(direction*DeltaTime), true, &HitResult) == false)
	{
		if (HitResult.GetActor() != nullptr)
		{
			//UE_LOG(LogDocumentationCode, Warning, TEXT("Cannot move object to location, blocked by %s"), *HitResult.GetActor()->GetName());
		}
	}
}

void ALaserProjectile::SetDirection(FVector dir) {
	direction = dir;
	direction[2] = 0;
	direction.Normalize();
	direction *= 4000;
	SetActorRotation(FRotator(90, 0, 0)+direction.Rotation());
}