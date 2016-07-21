// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "ItemSpawnPoint.h"



// Sets default values
AItemSpawnPoint::AItemSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ConstructorHelpers::FObjectFinder<UStaticMesh> sphere(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	if (sphere.Object) {
		mesh->SetStaticMesh(sphere.Object);
	}
	mesh->SetHiddenInGame(true);
}

// Called when the game starts or when spawned
void AItemSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemSpawnPoint::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

void AItemSpawnPoint::SpawnItem(EItemType type) {
	if (IsValid(currentSpawned)) {
		return;
	}
	UWorld *world = GetWorld();
	//ALaserProjectile* const Projectile = World->SpawnActor<ALaserProjectile>(ProjectileClass, location, bulletDirection.Rotation(), SpawnParams);
	AItemActor *newItem = world->SpawnActor<AItemActor>(AItemActor::StaticClass(), GetTransform());
	if (IsValid(newItem)) {
		newItem->SetSpawnPoint(this);
		newItem->SetItemType(type);
	}
}

void AItemSpawnPoint::SpawnItem() {
	UE_LOG(LogTemp, Warning, TEXT("Spawned item on %s"), *GetName());
}
