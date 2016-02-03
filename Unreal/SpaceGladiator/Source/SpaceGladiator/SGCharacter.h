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

	FTimerHandle RespawnTimerHandle;
	FTimerHandle ReviveTimerHandle;
	
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ALaserProjectile> ProjectileClass;

	/** Health */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Player)
		float Health;

	/** Color, randomly set in BP */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Player)
		FLinearColor Color;

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
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UClass *WallSegmentClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Replicated)
	AWallSegment *CurrentWall;
	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "SpaceGladiator")
		void PlaceWall();
	void PlaceWall_Implementation();
	bool PlaceWall_Validate();
	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "SpaceGladiator")
		void AddWallSegment();
	void AddWallSegment_Implementation();
	bool AddWallSegment_Validate();

	float TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) override;
	UFUNCTION(Reliable, NetMulticast)
	void explode();
	void explode_Implementation();
	UFUNCTION(BlueprintImplementableEvent, meta = (FriendlyName = "Explode"))
	void explodeEvent();
	void respawn();
	void revive();

	bool IsAlive();
};
