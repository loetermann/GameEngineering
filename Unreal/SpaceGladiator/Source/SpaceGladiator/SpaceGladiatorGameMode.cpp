// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "SpaceGladiatorPlayerController.h"
#include <algorithm>
#include <random>
#include "SpaceGladiatorGameMode.h"





ASpaceGladiatorGameMode::ASpaceGladiatorGameMode(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer) {
	PlayerControllerClass = ASpaceGladiatorPlayerController::StaticClass();
	HUDClass = ASGHUD::StaticClass();

	static ConstructorHelpers::FObjectFinder<UBlueprint> Character(TEXT("Blueprint'/Game/Player/SpaceGladiatorCharacter.SpaceGladiatorCharacter'"));
	if (Character.Object) {
		DefaultPawnClass = (UClass*)Character.Object->GeneratedClass;
	}
	//FILL VECTOR;
	colors.clear();
	colors.push_back(*(new FLinearColor(255, 255, 0))); // yellow
	colors.push_back(*(new FLinearColor(255, 0, 255))); // magenta
	colors.push_back(*(new FLinearColor(255, 0, 0))); //red
	colors.push_back(*(new FLinearColor(0, 255, 0))); //green
	colors.push_back(*(new FLinearColor(0, 0, 255))); // blue
	colors.push_back(*(new FLinearColor(0, 255, 255))); //cyan
	colors.push_back(*(new FLinearColor(100, 0, 255))); // purple
	//colors.push_back(*(new FLinearColor(0, 100, 255))); //light blue - possibly to similar to normal blue
	//colors.push_back(*(new FLinearColor(255, 100, 0))); // orange - too similar to yellow
	//colors.push_back(*(new FLinearColor(255, 0, 100))); // pinkish - too similar to purple/maganta
	//colors.push_back(*(new FLinearColor(100, 255, 0))); //light green - too similar to normal green
	//colors.push_back(*(new FLinearColor(0, 255, 100))); too similar to normal green

	
	auto engine = std::default_random_engine{};
	std::shuffle(std::begin(colors), std::end(colors), engine);

}

FLinearColor ASpaceGladiatorGameMode::getColor()
{
	if (colors.size() < 1)
		return *(new FLinearColor(255, 255, 255)); //WHITE, needs exception handling
	int r = std::rand() % colors.size();
	FLinearColor color = colors[r];
	colors.erase(colors.begin()+r);
	return color;
}

