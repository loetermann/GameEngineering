// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WallSegment.h"
#include "GameFramework/Character.h"
#include "ItemType.h"
#include "SGCharacter.generated.h"


UCLASS()
class SPACEGLADIATOR_API ASGCharacter : public ACharacter
{
	GENERATED_BODY()

		
		
public:

	FTimerHandle RespawnTimerHandle;
	FTimerHandle ReviveTimerHandle;
	FTimerHandle PunishTimerHandle;
	
	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
	TSubclassOf<class ALaserProjectile> ProjectileClass;

	/** Health */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Player)
		float Health;

	/** Shield */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Player)
		float Shield;
	
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Player)
		float MaxShield = 100;

	/** Color */
	UPROPERTY(ReplicatedUsing = ColorChanged, EditAnywhere, BlueprintReadWrite, Category = Player)
	FLinearColor Color;

	/** FireLoad */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Projectile)
		float FireLoad;

	/** MaxFireLoadTime */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Projectile)
		float MaxFireLoadTime;

	/** Canon */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* CanonComponent;
	UPROPERTY(AdvancedDisplay, Replicated, EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMesh* CanonMesh;
	UPROPERTY(AdvancedDisplay, Replicated, EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* CanonMaterial;

	/** Racer */
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* RacerComponent;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	UStaticMesh* RacerMesh;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Materials")
	UMaterialInterface* RacerMaterial;

	// Sets default values for this character's properties
	ASGCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "SpaceGladiator")
	void FireHold();
	void FireHold_Implementation();
	bool FireHold_Validate();
	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "SpaceGladiator")
	void Fire(FVector bulletDirection);
	void Fire_Implementation(FVector bulletDirection);
	bool Fire_Validate(FVector bulletDirection);

	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "SpaceGladiator")
	void FireWithLoad(FVector bulletDirection, float Load);
	void FireWithLoad_Implementation(FVector bulletDirection, float Load);
	bool FireWithLoad_Validate(FVector bulletDirection, float Load);

	UFUNCTION(Reliable, Server, WithValidation, BlueprintCallable, Category = "SpaceGladiator")
	void UseItem();
	void UseItem_Implementation();
	bool UseItem_Validate();

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

	UFUNCTION(BlueprintCallable, Category = "SpaceGladiator")
	float TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) override;
	UFUNCTION(Reliable, NetMulticast)
	void explode();
	void explode_Implementation();
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "Explode"))
	void explodeEvent();
	void respawn();
	void revive();
	void punish();

	UFUNCTION(BlueprintCallable, Category = "SpaceGladiator")
	bool IsAlive();

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "ColorChanged"))
		void colorChanged();

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	UFUNCTION(BlueprintCallable, Category = "SpaceGladiator")
	bool HasItem() { return ItemType != EItemType::ItemType_None; }
	void AddItem(EItemType type) { ItemType = type; }

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Player)
	EItemType ItemType;

	UPROPERTY(BlueprintReadOnly, Category = Player)
	float WallMaxTime;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = Player)
	float CurrentWallTime;
	UPROPERTY(BlueprintReadOnly, Category = Player)
	float WallCooldown;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = Player)
	float CurrentWallCooldown;



	UPROPERTY(BlueprintReadOnly, Category = Player)
	float LeftTimeForInvertControl;
	UPROPERTY(BlueprintReadOnly, Category = Player)
	float LeftTimeForInvertCamera;
	UPROPERTY(BlueprintReadOnly, Category = Player)
	float LeftTimeForAbsorbProjectiles;
	UPROPERTY(BlueprintReadOnly, Category = Player)
	float LeftTimeForUnstoppable;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = Player)
	bool IsControlInverted;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = Player)
	bool IsCameraReversed;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = Player)
	bool AbsorbsProjectiles;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = Player)
	bool IsUnstoppable;

private:
	void InitComponents();
	UFUNCTION(BlueprintCallable, Category = "SpaceGladiator")
	bool IsInvincible();
};
