// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
//#include "SGCharacter.h"
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

	UPROPERTY(BlueprintReadWrite, EditAnywhere, ReplicatedUsing = NextSegmentChanged)
	AWallSegment *NextSegment;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AWallSegment *PrevSegment;
		
	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//ASGCharacter *OwningCharacter;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UParticleSystemComponent *WallBeams;

	UFUNCTION(NetMulticast, Reliable)
	void SetBeamSource(AActor *source);
	void SetBeamSource_Implementation(AActor *source);

	UFUNCTION(NetMulticast, Reliable)
	void SetBeamTarget(AActor *target);
	void SetBeamTarget_Implementation(AActor *target);

	UFUNCTION()
	void NextSegmentChanged();

	//UPROPERTY(BlueprintReadWrite, EditAnywhere)
	//UBoxComponent *CollisionBox;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USceneComponent *RootScene;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USplineComponent *Spline;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	USplineMeshComponent *SplineMeshComponent;

	UPROPERTY(EditDefaultsOnly, Category="Spline")
	UStaticMesh *SplineMesh;

	UFUNCTION(NetMulticast, Reliable)
	void UpdateSplineLocation(FVector location);
	void UpdateSplineLocation_Implementation(FVector location);

	void UpdateSplineMesh();
	UFUNCTION()
	void OnBeginOverlap(AActor *OtherActor);

//	float IgnoreOverlapTime;
};
