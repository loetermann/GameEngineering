// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "SGAIController.h"
#include "SGPlayerState.h"
#include "SpaceGladiatorGameMode.h"

ASGAIController::ASGAIController(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer) {

	bWantsPlayerState = true;

}

void ASGAIController::BeginPlay() {
	Super::BeginPlay();
	if (HasAuthority()) {
		PlayerState->SetPlayerName(FString::Printf(TEXT("AI %d"), Cast<ASpaceGladiatorGameMode>(GetWorld()->GetAuthGameMode())->nextID()));
	}
}