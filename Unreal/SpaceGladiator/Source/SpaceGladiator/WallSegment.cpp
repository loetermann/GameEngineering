// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "WallSegment.h"


// Sets default values
AWallSegment::AWallSegment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	RootComponent = RootScene;
	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	Spline->AttachParent = RootScene;
	Spline->SetLocationAtSplinePoint(0, FVector(0, 0, 0), ESplineCoordinateSpace::Local);
	Spline->SetLocationAtSplinePoint(1, FVector(0, 0, 0), ESplineCoordinateSpace::Local);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SplineMesh(TEXT("StaticMesh'/Engine/EngineMeshes/Cube.Cube'"));
	if (SplineMesh.Object) {
		this->SplineMesh = SplineMesh.Object;
	}

	
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

}

void AWallSegment::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
	this->SetActorEnableCollision(false);
	UpdateSplineMesh();
}

void AWallSegment::UpdateSplineMesh() {
	UE_LOG(LogTemp, Warning, TEXT("OnConstruction %d"), Spline->GetNumberOfSplinePoints());
	//for (int i = 0; i < Spline->GetNumberOfSplinePoints() - 1; i++) {
	int i = 0;
		UE_LOG(LogTemp, Warning, TEXT("Adding Spline Mesh!!!!"));
		if (!SplineMeshComponent) {
			SplineMeshComponent = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());//<USplineMeshComponent>(USplineMeshComponent::StaticClass(), this);
		}

		SplineMeshComponent->SetStaticMesh(this->SplineMesh);
		SplineMeshComponent->SetStartScale(FVector2D(0.25, 1));
		SplineMeshComponent->SetEndScale(FVector2D(0.25, 1));
		SplineMeshComponent->AttachParent = Spline;
		
		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SplineMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SplineMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		SplineMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);

		
		SplineMeshComponent->SetMobility(EComponentMobility::Movable);
		
		//newSplineMesh->SetCollisionProfileName()

		FVector pStart, tangentStart, pEnd, tangentEnd;
		Spline->GetLocalLocationAndTangentAtSplinePoint(i, pStart, tangentStart);
		Spline->GetLocalLocationAndTangentAtSplinePoint(i + 1, pEnd, tangentEnd);

		SplineMeshComponent->SetStartAndEnd(pStart, tangentStart, pEnd, tangentEnd);
		SplineMeshComponent->RecreateCollision();

	//}
	RegisterAllComponents();
}
