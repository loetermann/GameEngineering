// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "UnrealNetwork.h"
#include "ItemActor.h"
#include "ItemSpawnPoint.h"
#include "SGCharacter.h"

// Sets default values
AItemActor::AItemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetActorEnableCollision(true);
	bReplicates = true;
	YawPerSeconds = 150.0f;
	ItemType = EItemType::ItemType_Magnet;
	// Our root component will be a sphere that reacts to physics
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	RootComponent = SphereComponent;
	float SphereRadius = 100.0f;
	SphereComponent->InitSphereRadius(SphereRadius);
	SphereComponent->SetRelativeLocation(FVector(0.0f, 0.0f, SphereRadius));
	SphereComponent->SetCollisionProfileName(TEXT("ItemCage"));
	SphereComponent->SetNetAddressable();
	SphereComponent->SetIsReplicated(true);

	Cage = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cage"));
	Cage->AttachTo(RootComponent);
	Cage->SetNetAddressable();
	Cage->SetIsReplicated(true);
		
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CageFinder(TEXT("StaticMesh'/Game/Items/ItemCage.ItemCage'"));
	if (CageFinder.Succeeded()) {
		CageMesh = CageFinder.Object;
		Cage->SetStaticMesh(CageMesh);
	}

	
	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> CageMaterialFinder(TEXT("MaterialInstanceConstant'/Game/Materials/CageMaterial.CageMaterial'"));
	if (CageMaterialFinder.Succeeded()) {
		CageMaterial = UMaterialInstanceDynamic::Create(CageMaterialFinder.Object, NULL);
		CageMaterial->SetVectorParameterValue("GlowColor", ItemColors[(uint8)ItemType]);
		Cage->SetMaterial(0, CageMaterial);
	}

	Cage->SetRelativeLocation(FVector(0.0f, 0.0f, -SphereRadius));
	Cage->SetRelativeScale3D(FVector(2.7f, 2.7f, 2.7f));

	Item = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item"));
	Item->AttachTo(SphereComponent);
	Item->SetNetAddressable();
	Item->SetIsReplicated(true);
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ItemFinder(TEXT("StaticMesh'/Game/Items/Item.Item'"));
	if (ItemFinder.Succeeded()) {
		ItemMesh = ItemFinder.Object;
		Item->SetStaticMesh(ItemMesh);
	}

	static ConstructorHelpers::FObjectFinder<UMaterialInstanceConstant> ItemMaterialFinder(TEXT("MaterialInstanceConstant'/Game/Materials/ItemMaterials/ItemMaterial.ItemMaterial'"));
	if (ItemMaterialFinder.Succeeded()) {
		ItemMaterial = UMaterialInstanceDynamic::Create(ItemMaterialFinder.Object, NULL);
		ItemMaterial->SetVectorParameterValue("GlowColor", ItemColors[(uint8)ItemType]);
		Item->SetMaterial(0, ItemMaterial);
	}

	Item->SetRelativeLocation(FVector(0.0f, 0.0f, -SphereRadius));
	Item->SetRelativeScale3D(FVector(2.7f, 2.7f, 2.7f));

	SphereComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComponent->SetCollisionObjectType(ECC_Pawn);

	SphereComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	
	if (Item && Cage) {
		Item->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Cage->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItemActor::OnBeginOverlap);
}

// Called when the game starts or when spawned
void AItemActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AItemActor::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	Item->AddRelativeRotation(FRotator(0.0f, DeltaTime * YawPerSeconds, 0.0f));
}

void AItemActor::OnBeginOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor(1.0, 1.0, 1.0), "Overlap");

	if (OtherActor->GetClass()->IsChildOf(ASGCharacter::StaticClass())) {
		ASGCharacter* SpaceGladiator = (ASGCharacter*)OtherActor;
		if (!SpaceGladiator->HasItem()) {
			SpaceGladiator->AddItem(ItemType);

			if (spawnPoint != NULL) {
				((AItemSpawnPoint*)spawnPoint)->currentSpawned = NULL;
			}
			Destroy();
			// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor(1.0, 1.0, 1.0), "New Item");
		}
	}
}

void AItemActor::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) 
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	
	ChangeMaterial();
}

void AItemActor::PostActorCreated()
{
	Super::PostActorCreated();
	
	ChangeMaterial();
}

void AItemActor::ChangeMaterial() 
{
	UTexture* ItemBitmap = Cast<UTexture>(StaticLoadObject(UTexture::StaticClass(), NULL, ItemGlowTextures[(uint8)ItemType]));
	if (IsValid(ItemMaterial) && IsValid(CageMaterial) && IsValid(ItemBitmap)) {
		ItemMaterial->SetTextureParameterValue("GlowMask", ItemBitmap);

		ItemMaterial->SetVectorParameterValue("GlowColor", ItemColors[(uint8)ItemType]);
		CageMaterial->SetVectorParameterValue("GlowColor", ItemColors[(uint8)ItemType]);
	}
}

void AItemActor::SetItemType(EItemType NewItemType)
{
	ItemType = NewItemType;
	ChangeMaterial();
}

void AItemActor::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(AItemActor, ItemType);
	DOREPLIFETIME(AItemActor, Cage);
	DOREPLIFETIME(AItemActor, CageMaterial);
	DOREPLIFETIME(AItemActor, CageMesh);
	DOREPLIFETIME(AItemActor, Item);
	DOREPLIFETIME(AItemActor, ItemMesh);
	DOREPLIFETIME(AItemActor, ItemMaterial);
	DOREPLIFETIME(AItemActor, SphereComponent);
}

void AItemActor::SetSpawnPoint(AActor *p) {
	spawnPoint = p;
	((AItemSpawnPoint*)p)->currentSpawned = this;
}