// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "SpaceGladiatorPlayerController.h"
#include "SGCharacter.h"

void ASpaceGladiatorPlayerController::BeginPlayingState() {
	APawn *pawn = GetPawn();
	if (IsValid(pawn)) {
		UStaticMeshComponent* targeting = (UStaticMeshComponent*)pawn->FindComponentByClass(UStaticMeshComponent::StaticClass());
		if (IsValid(targeting)) {
			targeting->SetHiddenInGame(false);
		}
	}
}

void ASpaceGladiatorPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("TurnLeft"), IE_Pressed, this, &ASpaceGladiatorPlayerController::TurnLeft);
	InputComponent->BindAction(TEXT("TurnRight"), IE_Pressed, this, &ASpaceGladiatorPlayerController::TurnRight);

	InputComponent->BindAxis(TEXT("Turn"), this, &ASpaceGladiatorPlayerController::Turn);
	InputComponent->BindAxis(TEXT("Tilt"), this, &ASpaceGladiatorPlayerController::TiltCamera);

	InputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &ASpaceGladiatorPlayerController::Fire);
	InputComponent->BindAction(TEXT("Recall"), IE_Pressed, this, &ASpaceGladiatorPlayerController::Recall);

	InputComponent->BindAction(TEXT("ToggleWall"), IE_Pressed, this, &ASpaceGladiatorPlayerController::PlaceWall);
}

void ASpaceGladiatorPlayerController::TurnLeft() {
	UE_LOG(LogTemp, Warning, TEXT("TurnLeft"));
	//APawn *Pawn = GetControlledPawn();
	//Pawn->set
	FRotator rotation = ControlRotation;
	rotation.Add(0, -90.0f, 0);
	SetControlRotation(rotation);
	FRotator rot = FRotator(0, 90.0f, 0);
	RotateCamera(rot);
	RotateTargeting(rot);
	AddWallSegment();
}

void ASpaceGladiatorPlayerController::TurnRight() {
	UE_LOG(LogTemp, Warning, TEXT("TurnRight"));
	FRotator rotation = ControlRotation;
	rotation.Add(0, 90.0f, 0);
	SetControlRotation(rotation);
	FRotator rot = FRotator(0, -90.0f, 0);
	RotateCamera(rot);
	RotateTargeting(rot);
	AddWallSegment();
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
			rot.Roll = 0;
			springArm->SetRelativeRotation(rot);
		}
	}
}

void ASpaceGladiatorPlayerController::RotateTargeting(FRotator Rotation)
{
	APawn *pawn = GetPawn();
	if (IsValid(pawn)) {
		UStaticMeshComponent* targeting = (UStaticMeshComponent*)pawn->FindComponentByClass(UStaticMeshComponent::StaticClass());
		if (IsValid(targeting)) {
			targeting->AddRelativeRotation(Rotation);
		}
	}
}



void ASpaceGladiatorPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);
	APawn *pawn = GetPawn();
	if (IsValid(pawn)) {
		pawn->GetMovementComponent()->AddInputVector(pawn->GetActorForwardVector());
	}
	if (IsValid(CurrentWall) && IsValid(pawn)) {
		CurrentWall->Spline->SetLocationAtSplinePoint(1, pawn->GetActorLocation() - 100 * pawn->GetActorForwardVector(),ESplineCoordinateSpace::World);
		CurrentWall->UpdateSplineMesh();
	}
}

void ASpaceGladiatorPlayerController::Fire() {
	ASGCharacter* Character = Cast<ASGCharacter>(GetPawn());
	// Get the camera transform
	FRotator CameraRot;
	USpringArmComponent* springArm = (USpringArmComponent*)Character->FindComponentByClass(USpringArmComponent::StaticClass());
	if (IsValid(springArm)) {
		CameraRot = springArm->GetComponentRotation();
	}
	Character->Fire(CameraRot.Vector());
}

void ASpaceGladiatorPlayerController::Recall() {
	Cast<ASGCharacter>(GetPawn())->RecallProjectiles();
}

void ASpaceGladiatorPlayerController::PlaceWall() {
	if (CurrentWall) {
		CurrentWall = 0;
		return;
	}
	CurrentWall = Cast<AWallSegment>(GetWorld()->SpawnActor(AWallSegment::StaticClass()));
	CurrentWall->SetActorEnableCollision(true);
	

	APawn *pawn = GetPawn();
	if (IsValid(pawn)) {
		CurrentWall->SetActorLocation(pawn->GetActorLocation());
		CurrentWall->Spline->SetLocationAtSplinePoint(0, pawn->GetActorLocation() - 100 * pawn->GetActorForwardVector(),ESplineCoordinateSpace::World);
		CurrentWall->Spline->SetLocationAtSplinePoint(1, pawn->GetActorLocation() - 100 * pawn->GetActorForwardVector(), ESplineCoordinateSpace::World);
		CurrentWall->UpdateSplineMesh();
		CurrentWall->OwningCharacter = Cast<ASGCharacter>(pawn);
	}
}

void ASpaceGladiatorPlayerController::AddWallSegment() {
	if (!CurrentWall) {
		return;
	}
	AWallSegment *newWallSegment = Cast<AWallSegment>(GetWorld()->SpawnActor(AWallSegment::StaticClass()));
	newWallSegment->SetActorEnableCollision(true);
	newWallSegment->PrevSegment = CurrentWall;
	CurrentWall->NextSegment = newWallSegment;

	APawn *pawn = GetPawn();
	if (IsValid(pawn)) {
		newWallSegment->SetActorLocation(pawn->GetActorLocation());
		newWallSegment->Spline->SetLocationAtSplinePoint(0, pawn->GetActorLocation() - 100 * pawn->GetActorForwardVector(), ESplineCoordinateSpace::World);
		newWallSegment->Spline->SetLocationAtSplinePoint(1, pawn->GetActorLocation() - 100 * pawn->GetActorForwardVector(), ESplineCoordinateSpace::World);
		newWallSegment->UpdateSplineMesh();
		newWallSegment->OwningCharacter = Cast<ASGCharacter>(pawn);
	}
	CurrentWall = newWallSegment;
}