// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "ItemActor.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
    ItemType_Bomb 			UMETA(DisplayName="Bomb"),
    ItemType_ClusterMine 	UMETA(DisplayName="Cluster Mine"),
	ItemType_Magnet			UMETA(DisplayName="Magnet")
};

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

	// Todo(Stephan): Check what Category=Enum does
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Enum)
	EItemType ItemType;
 

	/** Cage */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* CageComponent;
	UPROPERTY(AdvancedDisplay, Replicated, EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMesh* CageMesh;
	UPROPERTY(AdvancedDisplay, Replicated, EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* CageMaterial;

	/** ItemCoin */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* ItemCoinComponent;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMesh* ItemCoinMesh;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* ItemCoinMaterial;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

private:
	void InitComponents();

	
	
};
