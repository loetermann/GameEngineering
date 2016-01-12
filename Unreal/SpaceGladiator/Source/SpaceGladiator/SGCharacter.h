// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WallSegment.h"
#include "GameFramework/Character.h"
#include "SGCharacter.generated.h"

UCLASS()
class SPACEGLADIATOR_API ASGCharacter : public ACharacter
{
	GENERATED_BODY()
		
public:
	
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ALaserProjectile> ProjectileClass;

	// Sets default values for this character's properties
	ASGCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "SpaceGladiator")
	void Fire(FVector direction);
	void Fire_Implementation(FVector direction);
	bool Fire_Validate(FVector direction);

	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "SpaceGladiator")
	void RecallProjectiles();
	void RecallProjectiles_Implementation();
	bool RecallProjectiles_Validate();


	AWallSegment *CurrentWall;
	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "SpaceGladiator")
		void PlaceWall();
	void PlaceWall_Implementation();
	bool PlaceWall_Validate();
	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "SpaceGladiator")
		void AddWallSegment();
	void AddWallSegment_Implementation();
	bool AddWallSegment_Validate();
};
