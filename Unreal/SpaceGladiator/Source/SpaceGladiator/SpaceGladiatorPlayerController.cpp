// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "SpaceGladiatorPlayerController.h"
#include "SGCharacter.h"
#include "SpaceGladiatorGameMode.h"

ASpaceGladiatorPlayerController::ASpaceGladiatorPlayerController(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer) {
}

void ASpaceGladiatorPlayerController::BeginPlayingState() {
	if (HasAuthority()) {
		PlayerState->SetPlayerName(FString::Printf(TEXT("Player %d"), Cast<ASpaceGladiatorGameMode>(GetWorld()->GetAuthGameMode())->nextID()));
	}
	/*
	APawn *pawn = GetPawn();
	if (IsValid(pawn)) {
		UStaticMeshComponent* targeting = (UStaticMeshComponent*)pawn->FindComponentByClass(UStaticMeshComponent::StaticClass());
		if (IsValid(targeting)) {
			targeting->SetHiddenInGame(false);
		}
	}
	*/
}

void ASpaceGladiatorPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("TurnLeft"), IE_Pressed, this, &ASpaceGladiatorPlayerController::TurnLeft);
	InputComponent->BindAction(TEXT("TurnRight"), IE_Pressed, this, &ASpaceGladiatorPlayerController::TurnRight);

	InputComponent->BindAxis(TEXT("Turn"), this, &ASpaceGladiatorPlayerController::Turn);
	InputComponent->BindAxis(TEXT("Tilt"), this, &ASpaceGladiatorPlayerController::TiltCamera);

	InputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ASpaceGladiatorPlayerController::FireHold);
	InputComponent->BindAction(TEXT("Fire"), IE_Released, this, &ASpaceGladiatorPlayerController::Fire);
	InputComponent->BindAction(TEXT("Recall"), IE_Pressed, this, &ASpaceGladiatorPlayerController::Recall);

	InputComponent->BindAction(TEXT("ToggleWall"), IE_Pressed, this, &ASpaceGladiatorPlayerController::PlaceWall);
	InputComponent->BindAxis(TEXT("SnapCanon"));
	InputComponent->BindAction(TEXT("UseItem"), IE_Pressed, this, &ASpaceGladiatorPlayerController::UseItem);
}

void ASpaceGladiatorPlayerController::TurnLeft() {
	if (!Cast<ASGCharacter>(GetPawn())->IsAlive()) {
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("TurnLeft"));
	//APawn *Pawn = GetControlledPawn();
	//Pawn->set
	FRotator rotation = ControlRotation;
	rotation.Add(0, -90.0f, 0);
	SetControlRotation(rotation);
	FRotator rot = FRotator(0, 90.0f, 0);
	RotateCamera(rot);
	RotateTargeting(rot);

	Cast<ASGCharacter>(GetPawn())->AddWallSegment();
}

void ASpaceGladiatorPlayerController::TurnRight() {
	if (!Cast<ASGCharacter>(GetPawn())->IsAlive()) {
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("TurnRight"));
	FRotator rotation = ControlRotation;
	rotation.Add(0, 90.0f, 0);
	SetControlRotation(rotation);
	FRotator rot = FRotator(0, -90.0f, 0);
	RotateCamera(rot);
	RotateTargeting(rot);
	Cast<ASGCharacter>(GetPawn())->AddWallSegment();
}

void ASpaceGladiatorPlayerController::Turn(float Value) {
	FRotator rot = FRotator(0,10.0f * Value, 0);
		RotateCamera(rot);
		RotateTargeting(rot);
}

void ASpaceGladiatorPlayerController::TiltCamera(float Value) {
	RotateCamera(FRotator(10.0f * Value, 0, 0));
}

void ASpaceGladiatorPlayerController::RotateCamera(FRotator Rotation) {
	APawn *pawn = GetPawn();
	if (IsValid(pawn)) {
		USpringArmComponent* springArm = (USpringArmComponent*)pawn->FindComponentByClass(USpringArmComponent::StaticClass());
		if (IsValid(springArm)) {
			springArm->AddRelativeRotation(Rotation);
			FTransform transform = springArm->GetRelativeTransform();
			FRotator rot = transform.Rotator();
			if (rot.Pitch < -75) {
				rot.Pitch = -75;
			}
			else if (rot.Pitch > 0) {
				rot.Pitch = 0;
			}
			if (Cast<ASGCharacter>(GetPawn())->IsCameraReversed) {
				rot.Roll = 180;
			}
			else {
				rot.Roll = 0;
			}
			springArm->SetRelativeRotation(rot);
		}
	}
}

void ASpaceGladiatorPlayerController::RotateTargeting(FRotator Rotation) {
	APawn* pawn = GetPawn();
	ASGCharacter* character = Cast<ASGCharacter>(GetPawn());

	if (IsValid(pawn)) {
		UStaticMeshComponent* canon = character->CanonComponent;  // UStaticMeshComponent* targeting = (UStaticMeshComponent*)pawn->FindComponentByClass(UStaticMeshComponent::StaticClass());
		if (IsValid(canon)) {
			canon->AddRelativeRotation({0.0f, Rotation.Yaw, 0.0f});
		}
	}
}



void ASpaceGladiatorPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);
	APawn *pawn = GetPawn();
	if (IsValid(pawn) && Cast<ASGCharacter>(GetPawn())->IsAlive()) {
		pawn->GetMovementComponent()->AddInputVector(pawn->GetActorForwardVector());
	}
}

void ASpaceGladiatorPlayerController::FireHold() {
	Cast<ASGCharacter>(GetPawn())->FireHold();
}

void ASpaceGladiatorPlayerController::Fire() {
	ASGCharacter* Character = Cast<ASGCharacter>(GetPawn());
	// Get the camera transform
	FRotator CameraRot;
	if (InputComponent->GetAxisValue(TEXT("SnapCanon")) > 0) {
		CameraRot = ControlRotation;
	}
	else {
		USpringArmComponent* springArm = (USpringArmComponent*)Character->FindComponentByClass(USpringArmComponent::StaticClass());
		if (IsValid(springArm)) {
			CameraRot = springArm->GetComponentRotation();
		}
	}

	Character->Fire(CameraRot.Vector());
}

void ASpaceGladiatorPlayerController::Recall() {
	Cast<ASGCharacter>(GetPawn())->RecallProjectiles();
}

void ASpaceGladiatorPlayerController::PlaceWall() {
	Cast<ASGCharacter>(GetPawn())->PlaceWall();
}

bool ASpaceGladiatorPlayerController::IsPlacingWalls() {
	return IsValid(GetPawn()) && (Cast<ASGCharacter>(GetPawn())->CurrentWall);
}

void ASpaceGladiatorPlayerController::UseItem() {
	Cast<ASGCharacter>(GetPawn())->UseItem();
}