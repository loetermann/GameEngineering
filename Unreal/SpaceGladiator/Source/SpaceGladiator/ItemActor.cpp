// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "ItemActor.h"


// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ItemCoinComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemCoin"));
	ItemCoinComponent->RegisterComponent();
	ItemCoinComponent->AttachTo(RootComponent);

	// TODO(Stephan): Set Location and Scale for Cage and ItemCoin 
	// TODO(Stephan): Set Color and Item-Bitmap according to ItemType
	// TODO(Stephan): Make Items rotate
	// TODO(Stephan): Allow players to pick up items and store item field in SGCharacter
	// TODO(Stephan): Allow players to use items (first, do nothing)
	// TODO(Stephan): Implement different Item behaviours
	// TODO(Stephan): Think about new items

	static ConstructorHelpers::FObjectFinder<UStaticMesh> ItemCoinFinder(TEXT("StaticMesh'/Game/Meshes/ItemCoin.ItemCoin'"));

	if (ItemCoinFinder.Object) {
		ItemCoinComponent->StaticMesh = ItemCoinFinder.Object;
		ItemCoinMesh = ItemCoinFinder.Object;
	}
	/*
	ItemCoinComponent->SetRelativeScale3D(FVector(20.0f, 20.0f, 20.0f));
	ItemCoinComponent->SetRelativeRotation(FRotator(0, -90.0f, 0));
	*/

	CageComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cage"));
	CageComponent->RegisterComponent();
	CageComponent->AttachTo(RootComponent);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CageFinder(TEXT("StaticMesh'/Game/Meshes/Cage.Cage'"));

	if (CageFinder.Object) {
		CageComponent->StaticMesh = CageFinder.Object;
		CageMesh = CageFinder.Object;
	}
/*	
	CageComponent->SetRelativeRotation(FRotator(0, -90.0f, 0));
	CageComponent->SetRelativeScale3D(FVector(20.0f, 20.0f, 20.0f));
	CageComponent->SetRelativeLocation(FVector(-10.0f, -0.0f, 47.0f));
*/
}

void AItemActor::InitComponents() {
	if (ItemCoinMesh) { ItemCoinComponent->SetStaticMesh(ItemCoinMesh); } 
	if (CageMesh) { CageComponent->SetStaticMesh(CageMesh); }
	if (ItemCoinMaterial) { ItemCoinComponent->SetMaterial(0, ItemCoinMaterial); }
	if (CageMaterial) { CageComponent->SetMaterial(0, CageMaterial); }
 }

void AItemActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) { Super::PostEditChangeProperty(PropertyChangedEvent); InitComponents(); }


// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

