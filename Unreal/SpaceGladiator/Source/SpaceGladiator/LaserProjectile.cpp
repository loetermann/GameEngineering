// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "LaserProjectile.h"
#include "SGCharacter.h"
#include "UnrealNetwork.h"


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
			NoExplosion();
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

void ALaserProjectile::SetDirection(FVector dir, float speed) {
	direction = dir;
	direction[2] = 0;
	direction.Normalize();
	direction *= speed;
	SetActorRotation(FRotator(90, 0, 0) + direction.Rotation());
}

void ALaserProjectile::Hit(AActor* OtherActor) {
//	UE_LOG(LogTemp, Warning, TEXT("HIT %s"), *OtherActor->GetName());
	if (HasAuthority() && IsValid(GetOwner())) {
		if (GetOwner() != OtherActor) {
			if (OtherActor->GetClass()->IsChildOf(ASGCharacter::StaticClass())) {
				if(recalled) {
					Cast<ASGCharacter>(OtherActor)->TakeDamage(80, FDamageEvent(), Cast<ASGCharacter>(GetOwner())->GetController(), this);
				}
				else {
					Cast<ASGCharacter>(OtherActor)->TakeDamage(10, FDamageEvent(), Cast<ASGCharacter>(GetOwner())->GetController(), this);
				}
				Destroy();
			} else if (!OtherActor->GetClass()->IsChildOf(ALaserProjectile::StaticClass()) && OtherActor != Cast<ASGCharacter>(GetOwner())->CurrentWall) {
				Destroy();
			}
		}
	}
}

void ALaserProjectile::Recall() {
	recalled = true;
}

void ALaserProjectile::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ALaserProjectile, explodeOnDestroy);
}


void ALaserProjectile::NoExplosion_Implementation() {
	explodeOnDestroy = false;
}

bool ALaserProjectile::NoExplosion_Validate() {
	return true;
}
