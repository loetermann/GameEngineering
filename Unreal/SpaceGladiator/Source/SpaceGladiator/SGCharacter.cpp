// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "SGCharacter.h"
#include "LaserProjectile.h"
#include "UnrealNetwork.h"


// Sets default values
ASGCharacter::ASGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileClass = ALaserProjectile::StaticClass();
	SetActorEnableCollision(true);
	WallSegmentClass = AWallSegment::StaticClass();
	bReplicates = true;

}

// Called when the game starts or when spawned
void ASGCharacter::BeginPlay()
{
	Health = 100;
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
		AWallSegment *newWallSegment = Cast<AWallSegment>(GetWorld()->SpawnActor(WallSegmentClass));
		newWallSegment->IgnoreOverlapTime = 0.1;
		newWallSegment->SetActorRotation(GetActorRotation());
		newWallSegment->SetActorLocation(GetActorLocation());
		newWallSegment->SetActorEnableCollision(true);
		newWallSegment->PrevSegment = CurrentWall;
		newWallSegment->WallBeams->DestroyComponent();
		CurrentWall->NextSegment = newWallSegment;
		CurrentWall->IgnoreOverlapTime = 0.1f;
		CurrentWall->SetBeamTarget(newWallSegment);
		CurrentWall = 0;
		return;
	}
	CurrentWall = Cast<AWallSegment>(GetWorld()->SpawnActor(WallSegmentClass));
	CurrentWall->SetActorEnableCollision(true);

	CurrentWall->SetActorLocation(GetActorLocation());
	CurrentWall->SetActorRotation(GetActorRotation());
	CurrentWall->Spline->SetLocationAtSplinePoint(0, GetActorLocation(), ESplineCoordinateSpace::World);
	CurrentWall->Spline->SetLocationAtSplinePoint(1, GetActorLocation(), ESplineCoordinateSpace::World);
	CurrentWall->UpdateSplineMesh();
	CurrentWall->UpdateSplineStartLocation(GetActorLocation());
	CurrentWall->SetBeamTarget(this);
}
bool ASGCharacter::PlaceWall_Validate() {
	return true;
}

void ASGCharacter::AddWallSegment_Implementation() {
	if (!CurrentWall) {
		return;
	}
	AWallSegment *newWallSegment = Cast<AWallSegment>(GetWorld()->SpawnActor(WallSegmentClass));
	CurrentWall->NextSegment = newWallSegment;
	CurrentWall->SetBeamTarget(newWallSegment);
	CurrentWall = newWallSegment;
	newWallSegment->SetActorRotation(GetActorRotation());
	newWallSegment->SetActorEnableCollision(true);
	newWallSegment->PrevSegment = CurrentWall;
	
	newWallSegment->SetBeamSource(newWallSegment);
	newWallSegment->SetBeamTarget(this);
		newWallSegment->SetActorLocation(GetActorLocation());
		newWallSegment->Spline->SetLocationAtSplinePoint(0, GetActorLocation(), ESplineCoordinateSpace::World);
		newWallSegment->Spline->SetLocationAtSplinePoint(1, GetActorLocation(), ESplineCoordinateSpace::World);
		newWallSegment->UpdateSplineMesh();
		newWallSegment->UpdateSplineStartLocation(GetActorLocation());
	

}

bool ASGCharacter::AddWallSegment_Validate() {
	return true;
}

float ASGCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {
	// Call the base class - this will tell us how much damage to apply  
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	Health -= ActualDamage;
	if (Health <= 0) {
		explode();
		respawn();
	}
	return ActualDamage;
}

void ASGCharacter::explode_Implementation() {
	explodeEvent();
}

void ASGCharacter::respawn() {
	//GetWorld()->GetAuthGameMode()->RestartPlayer(GetController());
	Health = 100;
	SetActorLocation(GetWorld()->GetAuthGameMode()->ChoosePlayerStart(GetController())->GetActorLocation());
}


void ASGCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ASGCharacter, Health);
	DOREPLIFETIME(ASGCharacter, Color);
}