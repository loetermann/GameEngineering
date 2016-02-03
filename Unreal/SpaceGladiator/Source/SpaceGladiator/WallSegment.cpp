// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "SGCharacter.h"
#include "WallSegment.h"
#include "UnrealNetwork.h"


// Sets default values
AWallSegment::AWallSegment()
{
//	IgnoreOverlapTime = 0.0f;
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateMovement = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->AttachParent = RootScene;
//	Spline->SetLocationAtSplinePoint(0, FVector(0, 0, 0), ESplineCoordinateSpace::Local);
//	Spline->SetLocationAtSplinePoint(1, FVector(0, 0, 0), ESplineCoordinateSpace::Local);

	WallBeams = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BeamParticles"));
	WallBeams->AttachParent = RootScene;
	FParticleSysParam SourceParam = FParticleSysParam();
	SourceParam.Name = FName(TEXT("Source"));
	SourceParam.ParamType = EParticleSysParamType::PSPT_Actor;
	WallBeams->InstanceParameters.Add(SourceParam);
	FParticleSysParam TargetParam = FParticleSysParam();
	TargetParam.Name = FName(TEXT("Target"));
	TargetParam.ParamType = EParticleSysParamType::PSPT_Actor;
	WallBeams->InstanceParameters.Add(TargetParam);
	FParticleSysParam BeamColor = FParticleSysParam();
	BeamColor.Name = FName(TEXT("Color"));
	BeamColor.ParamType = EParticleSysParamType::PSPT_Color;
	BeamColor.Color = FColor(255, 255, 255);
	WallBeams->InstanceParameters.Add(BeamColor);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleTemplate(TEXT("ParticleSystem'/Game/Particles/P_Beam.P_Beam'"));
	if (ParticleTemplate.Object) {
		WallBeams->SetTemplate(ParticleTemplate.Object);
	}


	// static ConstructorHelpers::FObjectFinder<UStaticMesh> SplineMesh(TEXT("StaticMesh'/Engine/EngineMeshes/Cube.Cube'")); 
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SplineMesh(TEXT("StaticMesh'/Game/Meshes/WallMesh.WallMesh'"));
	if (SplineMesh.Object) {
		this->SplineMesh = SplineMesh.Object;
	}
	OnActorBeginOverlap.AddDynamic(this,&AWallSegment::OnBeginOverlap);
	this->SetActorEnableCollision(true);
	Spline->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Spline->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	Spline->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

}

// Called when the game starts or when spawned
void AWallSegment::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWallSegment::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	//IgnoreOverlapTime -= DeltaTime;
	//IgnoreOverlapTime = IgnoreOverlapTime < 0 ? 0.0f : IgnoreOverlapTime;
}

void AWallSegment::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
	this->SetActorEnableCollision(false);

	UpdateSplineMesh();
}

void AWallSegment::UpdateSplineMesh() {
	//if (HasAuthority()) {
		//UE_LOG(LogTemp, Warning, TEXT("updated on server"));
	//}

	//UE_LOG(LogTemp, Warning, TEXT("Adding Spline Mesh!!!!"));
	
	if (!SplineMeshComponent) {
		SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());//<USplineMeshComponent>(USplineMeshComponent::StaticClass(), this);
	}

	SplineMeshComponent->SetStaticMesh(this->SplineMesh);
	SplineMeshComponent->SetStartScale(FVector2D(0.1, 0.5));
	SplineMeshComponent->SetEndScale(FVector2D(0.1, 0.5));
	SplineMeshComponent->AttachParent = Spline;

	SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SplineMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	SplineMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	

	SplineMeshComponent->SetMobility(EComponentMobility::Movable);

	//newSplineMesh->SetCollisionProfileName()

	FVector pStart, tangentStart, pEnd, tangentEnd;
	Spline->GetLocalLocationAndTangentAtSplinePoint(0, pStart, tangentStart);
	Spline->GetLocalLocationAndTangentAtSplinePoint(1, pEnd, tangentEnd);

	SplineMeshComponent->SetStartAndEnd(pStart, tangentStart, pEnd, tangentEnd);
	SplineMeshComponent->RecreateCollision();
	RegisterAllComponents();

}

void AWallSegment::UpdateSplineLocation_Implementation(FVector location) {
	Spline->SetLocationAtSplinePoint(1, location, ESplineCoordinateSpace::World);
	UpdateSplineMesh();
}

void AWallSegment::SetBeamSource_Implementation(AActor *source) {
	if (IsValid(WallBeams)) {
		WallBeams->InstanceParameters[0].Actor = source;
	}
}

void AWallSegment::SetBeamTarget_Implementation(AActor *target) {
	if (IsValid(WallBeams)) {
		if (IsValid(target)) {
			WallBeams->InstanceParameters[1].Actor = target;
		}
	}
}

void AWallSegment::NextSegmentChanged() {
	if (IsValid(WallBeams)) {
		if (IsValid(NextSegment)) {
			WallBeams->InstanceParameters[1].Actor = NextSegment;
		}
	}
}


void AWallSegment::OnBeginOverlap(AActor *OtherActor) {
	//if (IgnoreOverlapTime > 0.0f) {
	//	return;
	//}
	if (OtherActor->GetClass()->IsChildOf(ASGCharacter::StaticClass())) {
		ASGCharacter *ref = (ASGCharacter*)OtherActor;
		if (ref->CurrentWall != this && IsValid(GetOwner())) {
			ref->TakeDamage(100, FDamageEvent(), Cast<ASGCharacter>(GetOwner())->GetController(), this);
		}
	}
}

void AWallSegment::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(AWallSegment, NextSegment);
}

void AWallSegment::SetBeamColor(FColor Color) {
	if (IsValid(WallBeams)) {
		WallBeams->InstanceParameters[2].Color = Color;
	}
}