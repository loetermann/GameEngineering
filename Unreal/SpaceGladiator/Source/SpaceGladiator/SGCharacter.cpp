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
	RacerComponent->AttachTo(RootComponent);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> RacerFinder(TEXT("StaticMesh'/Game/Meshes/Racer.Racer'"));

	if (RacerFinder.Object) {
		RacerComponent->StaticMesh = RacerFinder.Object;
		RacerMesh = RacerFinder.Object;
	}
	RacerComponent->SetRelativeScale3D(FVector(20.0f, 20.0f, 20.0f));
	RacerComponent->SetRelativeRotation(FRotator(0, -90.0f, 0));
	CanonComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Canon"));
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
	Shield = 0;
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
		if (CurrentWallTime > WallMaxTime && HasAuthority()) {
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
	if (!IsAlive() || IsInvincible()) {
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


void ASGCharacter::FireWithLoad_Implementation(FVector bulletDirection, float Load) {
	if (!IsAlive() || IsInvincible()) {
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
			Projectile->SetDirection(bulletDirection, Projectile->MaxSpeed*(Load*0.8/MaxFireLoadTime+0.2));
		}
	}
}

bool ASGCharacter::FireWithLoad_Validate(FVector bulletDirection, float Load) {
	return true;
}

void ASGCharacter::Fire_Implementation(FVector bulletDirection) {
		FireWithLoad(bulletDirection, FireLoad);
		FireLoad = 0;
}

bool ASGCharacter::Fire_Validate(FVector bulletDirection) {
	return true;
}



void ASGCharacter::RecallProjectiles_Implementation() {
	if (!IsAlive() || IsInvincible()) {
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
	if (!IsAlive() || IsInvincible() || CurrentWallCooldown) {
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
	if (!IsAlive() || IsInvincible()) {
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

void ASGCharacter::UseItem_Implementation() {
	
	if (HasItem()) {
		switch (ItemType) {
			case EItemType::ItemType_Magnet: {
				for (TActorIterator<ALaserProjectile> ActorItr(GetWorld()); ActorItr; ++ActorItr)
				{
					ActorItr->SetOwner(this);
					ActorItr->Recall();
				}
			} break;

			// TODO: Start a Timer and set back to normal for all Items 

			case EItemType::ItemType_InvertControls: {
				for (TActorIterator<ASGCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
				{
					ActorItr->HasReversedControlls = true;
				}
				// TODO: Rewire Controlls for Left and Right

			} break;

			case EItemType::ItemType_InvertCamera: {
				for (TActorIterator<ASGCharacter> ActorItr(GetWorld()); ActorItr; ++ActorItr)
				{
					ActorItr->IsCameraReversed = true;
				}
				// TODO: Actually flip camera

			} break;

			case EItemType::ItemType_ProjectileAbsorb: {
				AbsorbsProjectiles = true;
				// TODO: Handle the AbsorbsProjectiles Flag in TakeDamage

			} break;

			case EItemType::ItemType_Unstoppable: {
				// TODO: Handle the IsUnstoppable Flag in TakeDamage and Wall Collision
				IsUnstoppable = true;

			} break;
		}

		ItemType = EItemType::ItemType_None;
	}

	return;
}

bool ASGCharacter::UseItem_Validate() {
	return true;
}



float ASGCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {
	if (IsInvincible()&&DamageCauser!=this) {
		return 0;
	}
	if (!IsAlive() && HasAuthority()) {
		return 0;
	}
	// Call the base class - this will tell us how much damage to apply  
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (AbsorbsProjectiles && 
		(DamageCauser->GetClass()->IsChildOf(ALaserProjectile::StaticClass())))
	{
		Shield += ActualDamage;
		if (Shield > MaxShield) { Shield = MaxShield; }
	}
	else 
	{
		if (Shield > 0) {
			Shield -= ActualDamage;

			// Is there still damage to apply?
			if (Shield < 0) { 
				ActualDamage = -Shield;
				Shield = 0; 
			} else {
				ActualDamage = 0;
			}
		}

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


				FString message;
				if (EventInstigator == GetController()) {
					message = FString::Printf(TEXT("%s committed suicide"), *PlayerState->PlayerName);
				}
				else {
					message = FString::Printf(TEXT("%s killed %s with %s"), *EventInstigator->GetPawn()->PlayerState->PlayerName, *PlayerState->PlayerName, *DamageCauser->GetClass()->GetName());
				}


				GEngine->AddOnScreenDebugMessage(-1, 15.0f, Cast<ASGCharacter>(EventInstigator->GetPawn())->Color.ToFColor(false), message);
			}
			if (CurrentWall) {
				PlaceWall();
			}
			explode();
			for (TActorIterator<ALaserProjectile> ActorItr(GetWorld()); ActorItr; ++ActorItr)
			{
				if (ActorItr->GetOwner() == this) {
					ActorItr->Destroy();
				}
			}
			for (TActorIterator<AWallSegment> ActorItr(GetWorld()); ActorItr; ++ActorItr)
			{
				if (ActorItr->GetOwner() == this) {
					ActorItr->DestroyWall();
				}
			}
			SetActorHiddenInGame(true);
			SetActorEnableCollision(false);
			GetWorldTimerManager().SetTimer(RespawnTimerHandle, this, &ASGCharacter::respawn, 0.75);
		}
		
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
	AActor* spawnPoint = GetWorld()->GetAuthGameMode()->ChoosePlayerStart(GetController());
	SetActorRotation(spawnPoint->GetActorForwardVector().Rotation()); //FOR AI
	GetController()->SetControlRotation(spawnPoint->GetActorForwardVector().Rotation()); //FOR PLAYER
	SetActorLocation(spawnPoint->GetActorLocation());
	SetActorEnableCollision(true);
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

	GetWorldTimerManager().SetTimer(PunishTimerHandle, this, &ASGCharacter::punish, 12.5);
}

void ASGCharacter::punish() {
	if (IsInvincible()) {
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, Color.ToFColor(false), "YOU GOT PUNISHED");
		TakeDamage(100,FDamageEvent(),GetController(),this);
		
	}
	else{
//		GEngine->AddOnScreenDebugMessage(-1, 15.0f, Color.ToFColor(false), "you avoided punishment");

	}
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
	DOREPLIFETIME(ASGCharacter, ItemType);
	DOREPLIFETIME(ASGCharacter, Shield);
}

bool ASGCharacter::IsAlive() {
	return Health > 0;
}

bool ASGCharacter::IsInvincible() {
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, Color.ToFColor(false), FString::Printf(TEXT("Z: %f: %s"), Super::GetActorLocation().Z, *FString((Super::GetActorLocation().Z > 715) ? "true": "false")));
	return Super::GetActorLocation().Z > 715;
}
