// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "SpaceGladiatorPlayerController.h"
#include "SpaceGladiatorGameMode.h"




ASpaceGladiatorGameMode::ASpaceGladiatorGameMode(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer) {
	PlayerControllerClass = ASpaceGladiatorPlayerController::StaticClass();

	static ConstructorHelpers::FObjectFinder<UBlueprint> Character(TEXT("Blueprint'/Game/Player/SpaceGladiatorCharacter.SpaceGladiatorCharacter'"));
	if (Character.Object) {
		DefaultPawnClass = (UClass*)Character.Object->GeneratedClass;
	}
}