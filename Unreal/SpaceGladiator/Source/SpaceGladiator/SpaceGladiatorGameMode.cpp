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
	float scalar = 0.75;
	colors.push_back(*(new FLinearColor(100, 100, 0))*scalar); // yellow
	colors.push_back(*(new FLinearColor(100, 0, 100))*scalar); // magenta
	colors.push_back(*(new FLinearColor(200, 0, 0))*scalar); //red
	colors.push_back(*(new FLinearColor(0, 200, 0))*scalar); //green
	colors.push_back(*(new FLinearColor(0, 0, 200))*scalar); // blue
	colors.push_back(*(new FLinearColor(0, 100, 100))*scalar); //cyan
	//colors.push_back(*(new FLinearColor(100, 0, 255))); // purple too similar to magenta
	//colors.push_back(*(new FLinearColor(0, 100, 255))); //light blue - possibly to similar to normal blue
	//colors.push_back(*(new FLinearColor(255, 100, 0))); // orange - too similar to yellow
	//colors.push_back(*(new FLinearColor(255, 0, 100))); // pinkish - too similar to purple/maganta
	//colors.push_back(*(new FLinearColor(100, 255, 0))); //light green - too similar to normal green
	//colors.push_back(*(new FLinearColor(0, 255, 100))); // too similar to normal green

	
	auto engine = std::default_random_engine{};
	std::shuffle(std::begin(colors), std::end(colors), engine);

	id = 0;

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

int32 ASpaceGladiatorGameMode::nextID() {
	return id++;
}

