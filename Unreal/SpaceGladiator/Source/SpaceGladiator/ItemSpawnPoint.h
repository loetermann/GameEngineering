// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ItemActor.h"
#include "ItemType.h"
#include "ItemSpawnPoint.generated.h"

UCLASS()
class SPACEGLADIATOR_API AItemSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemSpawnPoint();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void SpawnItem(EItemType type);
	void SpawnItem();
	UPROPERTY(BlueprintReadWrite,Category="SpaceGladiator")
	UStaticMeshComponent *mesh;
	
	AActor *currentSpawned;
};
