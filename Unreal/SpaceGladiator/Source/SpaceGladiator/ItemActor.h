// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "UnrealNetwork.h"
#include "ItemType.h"
#include "ItemActor.generated.h"


UCLASS()
class SPACEGLADIATOR_API AItemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemActor();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditAnywhere, Category = "Properties")
	float YawPerSeconds;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Cage;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMesh* CageMesh;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInstanceDynamic* CageMaterial;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* Item;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMesh* ItemMesh;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInstanceDynamic* ItemMaterial;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Components")
	USphereComponent* SphereComponent;

	UPROPERTY(ReplicatedUsing = ChangeMaterial, EditAnywhere, BlueprintReadWrite, Category = Enum)
	EItemType ItemType;

	UFUNCTION()
	void OnBeginOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void PostActorCreated() override;

	UFUNCTION()
	void SetItemType(EItemType NewItemType);

	UFUNCTION()
	void ChangeMaterial();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	void SetSpawnPoint(AActor *p);
private:
	AActor *spawnPoint;
};
