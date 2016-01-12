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

	if (HasAuthority()) {
		//UE_LOG(LogTemp, Warning, TEXT("Ticking!!!!"));
	}
	if (IsValid(CurrentWall)) {
		CurrentWall->UpdateSplineLocation(GetActorLocation() - 100 * GetActorForwardVector());
		CurrentWall->Spline->SetLocationAtSplinePoint(1, GetActorLocation() - 100 * GetActorForwardVector(), ESplineCoordinateSpace::World);
		CurrentWall->UpdateSplineMesh();
	}
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


void ASGCharacter::PlaceWall_Implementation() {
	if (CurrentWall) {
		CurrentWall = 0;
		return;
	}
	CurrentWall = Cast<AWallSegment>(GetWorld()->SpawnActor(AWallSegment::StaticClass()));
	CurrentWall->SetActorEnableCollision(true);

		CurrentWall->SetActorLocation(GetActorLocation());
		CurrentWall->Spline->SetLocationAtSplinePoint(0, GetActorLocation() - 100 * GetActorForwardVector(), ESplineCoordinateSpace::World);
		CurrentWall->Spline->SetLocationAtSplinePoint(1, GetActorLocation() - 100 * GetActorForwardVector(), ESplineCoordinateSpace::World);
		CurrentWall->UpdateSplineMesh();
		CurrentWall->UpdateSplineStartLocation(GetActorLocation() - 100 * GetActorForwardVector());
}
bool ASGCharacter::PlaceWall_Validate() {
	return true;
}

void ASGCharacter::AddWallSegment_Implementation() {
	if (!CurrentWall) {
		return;
	}
	AWallSegment *newWallSegment = Cast<AWallSegment>(GetWorld()->SpawnActor(AWallSegment::StaticClass()));
	newWallSegment->SetActorEnableCollision(true);
	newWallSegment->PrevSegment = CurrentWall;
	CurrentWall->NextSegment = newWallSegment;

		newWallSegment->SetActorLocation(GetActorLocation());
		newWallSegment->Spline->SetLocationAtSplinePoint(0, GetActorLocation() - 100 * GetActorForwardVector(), ESplineCoordinateSpace::World);
		newWallSegment->Spline->SetLocationAtSplinePoint(1, GetActorLocation() - 100 * GetActorForwardVector(), ESplineCoordinateSpace::World);
		newWallSegment->UpdateSplineMesh();
		newWallSegment->UpdateSplineStartLocation(GetActorLocation() - 100 * GetActorForwardVector());
	
	CurrentWall = newWallSegment;
}

bool ASGCharacter::AddWallSegment_Validate() {
	return true;
}