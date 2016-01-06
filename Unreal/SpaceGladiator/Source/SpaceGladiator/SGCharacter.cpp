// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "SGCharacter.h"
#include "LaserProjectile.h"


// Sets default values
ASGCharacter::ASGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileClass = ALaserProjectile::StaticClass();
	SetActorEnableCollision(true);

}

// Called when the game starts or when spawned
void ASGCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASGCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ASGCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void ASGCharacter::Fire_Implementation(FVector direction) {
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		FVector location = GetActorLocation();
		location.Z = location.Z - 20;
		ALaserProjectile* const Projectile = World->SpawnActor<ALaserProjectile>(ProjectileClass, location, direction.Rotation(), SpawnParams);
		if (Projectile)
		{
			Projectile->SetDirection(direction);
		}
	}
}
bool ASGCharacter::Fire_Validate(FVector direction) {
	return true;
}

void ASGCharacter::RecallProjectiles_Implementation() {
	for (TActorIterator<ALaserProjectile> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->GetOwner() == this) {
			ActorItr->Recall();
		}
	}
}
bool ASGCharacter::RecallProjectiles_Validate() {
	return true;
}