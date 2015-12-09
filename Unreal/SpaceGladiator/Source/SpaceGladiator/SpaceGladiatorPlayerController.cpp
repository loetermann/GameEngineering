// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SpringArmComponent.h"
#include "SpaceGladiatorPlayerController.h"



void ASpaceGladiatorPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("TurnLeft"), IE_Pressed, this, &ASpaceGladiatorPlayerController::TurnLeft);
	InputComponent->BindAction(TEXT("TurnRight"), IE_Pressed, this, &ASpaceGladiatorPlayerController::TurnRight);

	InputComponent->BindAxis(TEXT("Turn"), this, &ASpaceGladiatorPlayerController::Turn);
	InputComponent->BindAxis(TEXT("Tilt"), this, &ASpaceGladiatorPlayerController::TiltCamera);
}

void ASpaceGladiatorPlayerController::TurnLeft() {
	UE_LOG(LogTemp, Warning, TEXT("TurnLeft"));
	//APawn *Pawn = GetControlledPawn();
	//Pawn->set
	FRotator rotation = ControlRotation;
	rotation.Add(0, -90.0f, 0);
	SetControlRotation(rotation);
	RotateCamera(FRotator(0,90.0f,0));
}

void ASpaceGladiatorPlayerController::TurnRight() {
	UE_LOG(LogTemp, Warning, TEXT("TurnRight"));
	FRotator rotation = ControlRotation;
	rotation.Add(0, 90.0f, 0);
	SetControlRotation(rotation);
	RotateCamera(FRotator(0,-90.0f,0));
}

void ASpaceGladiatorPlayerController::Turn(float Value) {
		RotateCamera(FRotator(0,10.0f * Value, 0));
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
		}
	}
}

void ASpaceGladiatorPlayerController::PlayerTick(float DeltaTime) {
	Super::PlayerTick(DeltaTime);
	APawn *pawn = GetPawn();
	if (IsValid(pawn)) {
		pawn->GetMovementComponent()->AddInputVector(pawn->GetActorForwardVector());
	}
}