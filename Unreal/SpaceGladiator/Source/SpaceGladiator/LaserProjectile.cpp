// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "LaserProjectile.h"
#include "SGCharacter.h"


// Sets default values
ALaserProjectile::ALaserProjectile(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Use a Capsule as a simple collision representation
	CollisionComp = ObjectInitializer.CreateDefaultSubobject<UCapsuleComponent>(this, TEXT("CapsuleComp"));
	CollisionComp->InitCapsuleSize(5.0f, 20.0f);
	RootComponent = CollisionComp;

	OnActorBeginOverlap.AddDynamic(this, &ALaserProjectile::Hit);
	recalled = false;
	explodeOnDestroy = true;
		
}

// Called when the game starts or when spawned
void ALaserProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(10);
}

// Called every frame
void ALaserProjectile::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	if (recalled) {
		direction = (GetOwner()->GetActorLocation() - GetActorLocation());
		if (direction.Size() < 4000*DeltaTime) {
			explodeOnDestroy = false;
			Destroy();
		}
		direction.Normalize();
		direction *= 4000;
		SetActorRotation(FRotator(90, 0, 0) + direction.Rotation());
	} else {
		direction *= (1-2*DeltaTime);
		if (direction.Size() < 1) {
			direction = FVector::ZeroVector;
		}
	}
	if (!SetActorLocation(GetActorLocation() + (direction*DeltaTime), true)) {
		if (HasAuthority()) {
			Destroy();
		}
	}
}

void ALaserProjectile::SetDirection(FVector dir) {
	direction = dir;
	direction[2] = 0;
	direction.Normalize();
	direction *= 4000;
	SetActorRotation(FRotator(90, 0, 0) + direction.Rotation());
}

void ALaserProjectile::Hit(AActor* OtherActor) {
//	UE_LOG(LogTemp, Warning, TEXT("HIT %s"), *OtherActor->GetName());
	if (HasAuthority()) {
		if (GetOwner() != OtherActor) {
			if (OtherActor->GetClass()->IsChildOf(ACharacter::StaticClass())) {
			}
			if (!OtherActor->GetClass()->IsChildOf(ALaserProjectile::StaticClass())) {
				Destroy();
			}
		}
	}
}

void ALaserProjectile::Recall() {
	recalled = true;
}