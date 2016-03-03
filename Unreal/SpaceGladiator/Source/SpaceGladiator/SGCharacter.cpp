// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "SGCharacter.h"
#include "LaserProjectile.h"
#include "SGPlayerState.h"
#include "UnrealNetwork.h"


// Sets default values
ASGCharacter::ASGCharacter()
{

	WallMaxTime = 10.0f;
	CurrentWallTime = 0.0f;
	WallCooldown = 5.0f;
	CurrentWallCooldown = 0.0f;

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileClass = ALaserProjectile::StaticClass();
	SetActorEnableCollision(true);
	WallSegmentClass = AWallSegment::StaticClass();
	bReplicates = true;

	RacerComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Racer"));
	RacerComponent->RegisterComponent();
	RacerComponent->AttachTo(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RacerFinder(TEXT("StaticMesh'/Game/Meshes/Racer.Racer'"));

	if (RacerFinder.Object) {
		RacerComponent->StaticMesh = RacerFinder.Object;
		RacerMesh = RacerFinder.Object;
	}
	RacerComponent->SetRelativeScale3D(FVector(20.0f, 20.0f, 20.0f));
	RacerComponent->SetRelativeRotation(FRotator(0, -90.0f, 0));
	CanonComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Canon"));
	CanonComponent->RegisterComponent();
	CanonComponent->AttachTo(RootComponent);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CanonFinder(TEXT("StaticMesh'/Game/Meshes/Canon.Canon'"));

	if (CanonFinder.Object) {
		CanonComponent->StaticMesh = CanonFinder.Object;
		CanonMesh = CanonFinder.Object;
	}
	CanonComponent->SetRelativeRotation(FRotator(0, -90.0f, 0));
	CanonComponent->SetRelativeScale3D(FVector(20.0f, 20.0f, 20.0f));
	CanonComponent->SetRelativeLocation(FVector(-10.0f, -0.0f, 47.0f));
	InitComponents();
}

void ASGCharacter::InitComponents() {
	if (RacerMesh) { RacerComponent->SetStaticMesh(RacerMesh); } 
	if (CanonMesh) { CanonComponent->SetStaticMesh(CanonMesh); }
	if (RacerMaterial) { RacerComponent->SetMaterial(0, RacerMaterial); }
	if (CanonMaterial) { CanonComponent->SetMaterial(0, CanonMaterial); }
 }

void ASGCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) { Super::PostEditChangeProperty(PropertyChangedEvent); InitComponents(); }

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
		if (GetActorLocation().Z < 600) {
			TakeDamage(100, FDamageEvent(), GetController(), this);
		}
		if (FireLoad != 0 && FireLoad < MaxFireLoadTime) {
			FireLoad += DeltaTime;
			if (FireLoad > MaxFireLoadTime) {
				FireLoad = MaxFireLoadTime;
			}
		}
	}
	if (IsValid(CurrentWall)) {
		CurrentWall->UpdateSplineLocation_Implementation(GetActorLocation());
		CurrentWallTime += DeltaTime;
		if (CurrentWallTime > WallMaxTime) {
			PlaceWall();
		}
	}
	CurrentWallCooldown -= DeltaTime;
	if (CurrentWallCooldown < 0.0f) {
		CurrentWallCooldown = 0.0f;
	}
}

// Called to bind functionality to input
void ASGCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void ASGCharacter::FireHold_Implementation() {
	if (!IsAlive()) {
		return;
	}
	FireLoad = 0.001;
}
bool ASGCharacter::FireHold_Validate() {
	return true;
}

inline bool IsCloseTo(float Rotation, float Degrees) {
	float Deviation = 3.0f;
	bool Result = ((Degrees - Deviation) < Rotation) && (Rotation < (Degrees + Deviation));

	return Result;
}

void ASGCharacter::Fire_Implementation(FVector bulletDirection) {
	if (!IsAlive()) {
		return;
	}
	UWorld* const World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		FVector location = GetActorLocation();

		location.Z = location.Z - 20;

		// Adjust for Canon offset so that Projectiles seem to be launched from there
		float CanonOffset = 50.0f;

		FRotator rotation = GetActorRotation();

		if (IsCloseTo(rotation.Yaw, 0.0f)) { location.X -= CanonOffset; }
		else if (IsCloseTo(rotation.Yaw, 90.0f)) { location.Y -= CanonOffset; }
		else if (IsCloseTo(rotation.Yaw, -180.0f)) { location.X += CanonOffset; }
		else if (IsCloseTo(rotation.Yaw, -90.0f)) { location.Y += CanonOffset; }

		ALaserProjectile* const Projectile = World->SpawnActor<ALaserProjectile>(ProjectileClass, location, bulletDirection.Rotation(), SpawnParams);
		if (Projectile)
		{
			Projectile->SetDirection(bulletDirection, MaxProjectileSpeed*FireLoad/MaxFireLoadTime);
		}
		FireLoad = 0;
	}
}

bool ASGCharacter::Fire_Validate(FVector bulletDirection) {
	return true;
}

void ASGCharacter::RecallProjectiles_Implementation() {
	if (!IsAlive()) {
		return;
	}
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
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;
		AWallSegment *newWallSegment = GetWorld()->SpawnActor<AWallSegment>(WallSegmentClass, GetActorLocation(), GetActorRotation(), SpawnParams);
//		newWallSegment->IgnoreOverlapTime = 0.1;
		newWallSegment->PrevSegment = CurrentWall;
		newWallSegment->WallBeams->DestroyComponent();
		newWallSegment->SetBeamColor(FColor(Color.R, Color.G, Color.B, Color.A));
		CurrentWall->NextSegment = newWallSegment;
//		CurrentWall->IgnoreOverlapTime = 0.1f;
		CurrentWall->SetBeamTarget(newWallSegment);
		CurrentWall = 0;
		return;
	}
	if (!IsAlive() || CurrentWallCooldown) {
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	CurrentWall = GetWorld()->SpawnActor<AWallSegment>(WallSegmentClass, GetActorLocation(), GetActorRotation(), SpawnParams);

	CurrentWall->SetBeamTarget(this);
	CurrentWall->SetActorEnableCollision(true);
	CurrentWall->SetBeamColor(FColor(Color.R,Color.G,Color.B,Color.A));

	CurrentWallTime = 0.0f;
	CurrentWallCooldown = WallCooldown;
}
bool ASGCharacter::PlaceWall_Validate() {
	return true;
}

void ASGCharacter::AddWallSegment_Implementation() {
	if (!IsAlive()) {
		return;
	}
	if (!CurrentWall) {
		return;
	}
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;
	AWallSegment *newWallSegment = GetWorld()->SpawnActor<AWallSegment>(WallSegmentClass, GetActorLocation(), GetActorRotation(), SpawnParams);
	CurrentWall->NextSegment = newWallSegment;
	CurrentWall->SetBeamTarget(newWallSegment);
	CurrentWall->UpdateSplineLocation(GetActorLocation());
	newWallSegment->PrevSegment = CurrentWall;
	newWallSegment->SetBeamColor(FColor(Color.R, Color.G, Color.B, Color.A));
	CurrentWall = newWallSegment;
	CurrentWall->SetActorEnableCollision(true);
	
	
	newWallSegment->SetBeamSource(newWallSegment);
	newWallSegment->SetBeamTarget(this);
	

}

bool ASGCharacter::AddWallSegment_Validate() {
	return true;
}

float ASGCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {
	if (!IsAlive() && HasAuthority()) {
		return 0;
	}
	// Call the base class - this will tell us how much damage to apply  
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser); 
	Health -= ActualDamage;
	if (Health <= 0) {
		if (IsValid(GetController()) && IsValid(EventInstigator) && IsValid(DamageCauser) && IsValid(EventInstigator->GetPawn()->PlayerState) && IsValid(PlayerState)) {
			if (EventInstigator->GetPawn() == this) {
				EventInstigator->GetPawn()->PlayerState->Score--;
			}
			else {
				Cast<ASGPlayerState>(EventInstigator->GetPawn()->PlayerState)->Kills++;
				EventInstigator->GetPawn()->PlayerState->Score++;
			}
			Cast<ASGPlayerState>(PlayerState)->Deaths++;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, Cast<ASGCharacter>(EventInstigator->GetPawn())->Color.ToFColor(false), FString::Printf(TEXT("%s was killed by %s with %s"), *PlayerState->PlayerName, *EventInstigator->GetPawn()->PlayerState->PlayerName, *DamageCauser->GetClass()->GetName()));
		}
		if (CurrentWall) {
			PlaceWall();
		}
		explode();
		SetActorHiddenInGame(true);
		SetActorEnableCollision(false);
		GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASGCharacter::respawn, 0.75);
	}
	return ActualDamage;
}

void ASGCharacter::explode_Implementation() {
	explodeEvent();
}

void ASGCharacter::respawn() {
	if (IsAlive()) {
		return;
	}
	SetActorLocation(GetWorld()->GetAuthGameMode()->ChoosePlayerStart(GetController())->GetActorLocation());
	SetActorHiddenInGame(false);
	GetWorldTimerManager().SetTimer(ReviveTimerHandle, this, &ASGCharacter::revive, 0.75);
}

void ASGCharacter::revive() {
	if (IsAlive()) {
		return;
	}
	//GetWorld()->GetAuthGameMode()->RestartPlayer(GetController());
	Health = 100;
	SetActorEnableCollision(true);
}


void ASGCharacter::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ASGCharacter, Health);
	DOREPLIFETIME(ASGCharacter, Color);
	DOREPLIFETIME(ASGCharacter, FireLoad);
	DOREPLIFETIME(ASGCharacter, MaxFireLoadTime);
	DOREPLIFETIME(ASGCharacter, CurrentWall);
}

bool ASGCharacter::IsAlive() {
	return Health > 0;
}
