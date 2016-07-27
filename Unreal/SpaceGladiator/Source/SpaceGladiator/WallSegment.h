// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
//#include "SGCharacter.h"
#include "WallSegment.generated.h"
#define CAPSULE_RADIUS 200
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

	UFUNCTION(BlueprintCallable, Category="Space Gladiator|Walls")
	void SetBeamColor(FColor Color);
//	float IgnoreOverlapTime;
	UPROPERTY(EditAnyWhere, Category="Space Gladiator|Walls")
	float TimeToLive;
	UPROPERTY(EditAnyWhere, Category = "Space Gladiator|Walls")
	float LifeTime;
	UPROPERTY(Replicated, EditAnyWhere, Category = "Space Gladiator|Walls")
	FColor InitialWallColor;
	UFUNCTION(BlueprintCallable, Category = "Space Gladiator|Walls")
	void DestroyWall();

	float BreakingCooldown;
	float BreakingCooldownLeft;

	float updateRepetition;
private:
	void BreakWallNew(AActor *breaker);
	void BreakWall(AActor *breaker);
	void BreakSegment(AWallSegment *breakingSegment, FVector breakLocation, int &NearResult);
	void ProjectOnSegment(FVector &o, FVector &r, FVector &p, float &t);
	AWallSegment *FindSegmentToBreak(bool forwardSearch, FVector breakLocation, int &NearResult);
	int IsSegmentNearBreakPoint(AWallSegment *segment, FVector &breakLocation);
};
