// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "SGAIController.h"

ASGAIController::ASGAIController(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer) {

	bWantsPlayerState = true;

}

void ASGAIController::BeginPlay() {
	Super::BeginPlay();
	PlayerState->SetPlayerName(GetName());
}