// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "SGCharacter.h"
#include "WallSegment.generated.h"

UCLASS()
class SPACEGLADIATOR_API AWallSegment : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWallSegment();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void OnConstruction(const FTransform &Transform) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AWallSegment *NextSegment;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AWallSegment *PrevSegment;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	ASGCharacter *OwningCharacter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent *RootScene;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USplineComponent *Spline;

	UPROPERTY(BlueprintReadOnly)
	USplineMeshComponent *SplineMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category="Spline")
	UStaticMesh *SplineMesh;

	void UpdateSplineMesh();
};
