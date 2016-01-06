// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "LaserProjectile.generated.h"

UCLASS()
class ALaserProjectile : public AActor
{
	GENERATED_BODY()

	FVector direction;
	bool recalled;
		
public:


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Projectile)
		bool explodeOnDestroy;

	/** Capsule collision component */
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UCapsuleComponent* CollisionComp;
	
	// Sets default values for this actor's properties
	ALaserProjectile(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	void SetDirection(FVector dir);

	UFUNCTION()
	void Hit(AActor* OtherActor);

	void Recall();
	
};
