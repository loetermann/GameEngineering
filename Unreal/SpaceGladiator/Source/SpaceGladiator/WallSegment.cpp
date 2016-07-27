// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "SGCharacter.h"
#include "WallSegment.h"
#include "UnrealNetwork.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AWallSegment::AWallSegment()
{
//	IgnoreOverlapTime = 0.0f;
	bReplicates = true;
	bAlwaysRelevant = true;
	bReplicateMovement = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TimeToLive = 30.0f;
	LifeTime = 0.0f;
	InitialWallColor = FColor(255, 255, 255, 255);
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

	BreakingCooldown = 0.1f;
	BreakingCooldownLeft = 0.0f;

	updateRepetition = 0.0f;
}

// Called when the game starts or when spawned
void AWallSegment::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AWallSegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LifeTime += DeltaTime;
	if (LifeTime > TimeToLive && LifeTime < TimeToLive + 0.5f) {
		//destroy Actor!
		DestroyWall();
	}
	else 	if (IsValid(WallBeams)) {
		FColor newColor = InitialWallColor;
		newColor.R = InitialWallColor.R - LifeTime * 0.8f / TimeToLive * InitialWallColor.R;
		newColor.G = InitialWallColor.G - LifeTime * 0.8f / TimeToLive * InitialWallColor.G;
		newColor.B = InitialWallColor.B - LifeTime * 0.8f / TimeToLive * InitialWallColor.B;
		newColor.A = InitialWallColor.A - LifeTime * 0.8f / TimeToLive * InitialWallColor.A;

		WallBeams->InstanceParameters[2].Color = newColor;
	}
	//IgnoreOverlapTime -= DeltaTime;
	//IgnoreOverlapTime = IgnoreOverlapTime < 0 ? 0.0f : IgnoreOverlapTime;
	if (BreakingCooldownLeft > 0.0f) {
		BreakingCooldownLeft -= DeltaTime;
	}
	else if(BreakingCooldownLeft < 0.0f)
	{
		BreakingCooldownLeft = 0.0f;
	}
	
	if (updateRepetition > 0.0f) {
		updateRepetition -= DeltaTime;
		UpdateSplineMesh();
	}
	else if (updateRepetition < 0.0f) {
		updateRepetition = 0.0f;
	}
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
	
	//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("UpdateSplineMesh %s"), *this->GetName()));

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
#if WITH_EDITOR
	SplineMeshComponent->RecreateCollision();
#endif
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
			//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%s overlapping with %s"), *this->GetName(),*ref->GetName()));
			if (true) {
				if (BreakingCooldownLeft == 0.0f) {
					BreakingCooldownLeft = BreakingCooldown;
					//BreakWall(ref);
					//UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("COLLISION %s"), *OtherActor->GetName()));
					BreakWallNew(ref);
				}
			}
			else {
				ref->TakeDamage(300, FDamageEvent(), Cast<ASGCharacter>(GetOwner())->GetController(), this);
			}
		}
	}
}

void AWallSegment::BreakWallNew(AActor *breaker) {
	AWallSegment *newWallSegment;
	FVector breakLocation = breaker->GetActorLocation();
	int result = IsSegmentNearBreakPoint(this, breakLocation);
	USplineComponent *spline;
	FVector o, r;
	float t;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this->GetOwner();
	SpawnParams.Instigator = Instigator;
	spline = this->Spline;
	o = spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
	r = (spline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::World) - o).GetSafeNormal();
	ProjectOnSegment(o, r, breakLocation, t);
	//DEBUG
	//DrawDebugPoint(GetWorld(), breakLocation + CAPSULE_RADIUS*r, 25, FColor::Green, false, 10.0f, 0);
	//DrawDebugPoint(GetWorld(), breakLocation - CAPSULE_RADIUS*r, 25, FColor::Blue, false, 10.0f, 0);
	//AWallSegment *savedPrevSegment = NULL;
	//AWallSegment *savedNextSegment = NULL;

	AWallSegment *exchangedSegment;
	//DEBUG END
	switch (result) {
	case 0:
		exchangedSegment = GetWorld()->SpawnActor<AWallSegment>(AWallSegment::StaticClass(), this->GetActorLocation(), GetActorRotation(), SpawnParams);
		exchangedSegment->SetActorEnableCollision(true);

		newWallSegment = GetWorld()->SpawnActor<AWallSegment>(AWallSegment::StaticClass(), breakLocation + CAPSULE_RADIUS*r, GetActorRotation(), SpawnParams);
		newWallSegment->NextSegment = this->NextSegment;
		if (IsValid(newWallSegment->NextSegment)) {
			newWallSegment->NextSegment->PrevSegment = newWallSegment;
		}
		newWallSegment->PrevSegment = NULL;
		newWallSegment->SetBeamColor(InitialWallColor);
		newWallSegment->SetBeamSource(newWallSegment);
		newWallSegment->SetBeamTarget(this->NextSegment);
		newWallSegment->SetActorEnableCollision(true);
		newWallSegment->Spline->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		if (IsValid(this->NextSegment)) {
			newWallSegment->UpdateSplineLocation(this->NextSegment->GetActorLocation());
			newWallSegment->updateRepetition = 0.5f;
		}

		newWallSegment = GetWorld()->SpawnActor<AWallSegment>(AWallSegment::StaticClass(), breakLocation - CAPSULE_RADIUS*r, GetActorRotation(), SpawnParams);
		newWallSegment->NextSegment = NULL;
		newWallSegment->PrevSegment = exchangedSegment;
		newWallSegment->WallBeams->DestroyComponent();
		newWallSegment->Spline->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		newWallSegment->Spline->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		exchangedSegment->NextSegment = newWallSegment;
		exchangedSegment->SetBeamTarget(newWallSegment);
		exchangedSegment->SetActorEnableCollision(true);
		exchangedSegment->UpdateSplineLocation(newWallSegment->GetActorLocation());
		exchangedSegment->updateRepetition = 0.5f;

		exchangedSegment->SetBeamColor(InitialWallColor);

		exchangedSegment->PrevSegment = this->PrevSegment;
		
		this->NextSegment = NULL;
		this->PrevSegment = NULL;
		this->DestroyWall();
		break;
	case 1: // start near
		this->SetActorLocation(breakLocation + CAPSULE_RADIUS * r);
		this->updateRepetition = 0.5f;
		break;
	case 2: // end near
		newWallSegment = GetWorld()->SpawnActor<AWallSegment>(AWallSegment::StaticClass(), breakLocation - CAPSULE_RADIUS*r, GetActorRotation(), SpawnParams);
		newWallSegment->NextSegment = NULL;
		newWallSegment->PrevSegment = this;
		newWallSegment->WallBeams->DestroyComponent();
		newWallSegment->Spline->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		newWallSegment->Spline->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		this->NextSegment = newWallSegment;
		this->SetBeamTarget(newWallSegment);
		this->SetActorEnableCollision(true);
		this->UpdateSplineLocation(newWallSegment->GetActorLocation());
		this->updateRepetition = 0.5f;
		break;
	case 3: // both near
		if (IsValid(this->NextSegment)) {
			this->NextSegment->PrevSegment = NULL;
		}
		if (IsValid(this->PrevSegment)) {
			this->PrevSegment->NextSegment = NULL;
		}
		this->DestroyWall();
		break;
	default:
		break;
	}
}

void AWallSegment::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(AWallSegment, NextSegment);
	DOREPLIFETIME(AWallSegment, InitialWallColor);
}

void AWallSegment::SetBeamColor(FColor Color) {
	/*
	if (IsValid(WallBeams)) {
		WallBeams->InstanceParameters[2].Color = Color;
	}*/
	InitialWallColor = Color;
}

void AWallSegment::DestroyWall() {
	AWallSegment *forwardWallRef = 0;
	//AWallSegment *backwardWallRef;
	int counter = 0;
	if (IsValid(NextSegment)) {
		forwardWallRef = NextSegment;

		while (IsValid(forwardWallRef->NextSegment)) {
			forwardWallRef = forwardWallRef->NextSegment;
			counter++;
			if (counter > 500000) {
				break;
			}
			if (IsValid(forwardWallRef->PrevSegment))
				forwardWallRef->PrevSegment->Destroy();
		}
		if (IsValid(forwardWallRef))
			forwardWallRef->Destroy();
	}
	if (IsValid(PrevSegment)) {
		forwardWallRef = PrevSegment;
		counter = 0;
		while (IsValid(forwardWallRef->PrevSegment)) {
			forwardWallRef = forwardWallRef->PrevSegment;
			counter++;
			if (counter > 500000) {
				break; 
			}
			if (IsValid(forwardWallRef->NextSegment))
				forwardWallRef->NextSegment->Destroy();
		}
		if (IsValid(forwardWallRef))
			forwardWallRef->Destroy();
	}
	this->Destroy();
}

void AWallSegment::BreakWall(AActor *breaker) {
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("%s broken by %s"), *this->GetName(), *breaker->GetName()));
	int NearResult = 0;
	AWallSegment *breakingSegment = FindSegmentToBreak(true, breaker->GetActorLocation(), NearResult);
	if (NearResult == 0) {
		BreakSegment(breakingSegment, breaker->GetActorLocation(), NearResult);
	}
	else {
		if (NearResult == 3) {
			AWallSegment *WallToDestroy = this->NextSegment;
			if (IsValid(NextSegment)) {
				NextSegment->PrevSegment = NULL;
				NextSegment->DestroyWall();
				this->NextSegment = NULL;
			}
		}
		else {
			AWallSegment *WallToDestory = this->NextSegment;
			if (IsValid(breakingSegment->PrevSegment) && IsValid(WallToDestory)) {
				breakingSegment->PrevSegment->NextSegment = NULL;
				breakingSegment->PrevSegment = NULL;
				WallToDestory->PrevSegment = NULL;
				WallToDestory->DestroyWall();
				this->NextSegment = NULL;
			}
			BreakSegment(breakingSegment, breaker->GetActorLocation(), NearResult);
		}
		if (breakingSegment != this) {
			breakingSegment = FindSegmentToBreak(false, breaker->GetActorLocation(), NearResult);
			if (NearResult == 3) {
				AWallSegment *WallToDestroy = this->PrevSegment;
				if (IsValid(PrevSegment)) {
					PrevSegment->NextSegment= NULL;
					PrevSegment->DestroyWall();
					this->PrevSegment= NULL;
				}
			}
			else {
				AWallSegment *WallToDestory = this->PrevSegment;
				if (IsValid(breakingSegment->NextSegment) && IsValid(WallToDestory)) {
					breakingSegment->NextSegment->PrevSegment = NULL;
					breakingSegment->NextSegment = NULL;
					WallToDestory->NextSegment = NULL;
					WallToDestory->DestroyWall();
					this->PrevSegment = NULL;
				}
				BreakSegment(breakingSegment, breaker->GetActorLocation(), NearResult);
			}
		}
	}
}

void AWallSegment::BreakSegment(AWallSegment *breakingSegment, FVector breakLocation, int &NearResult) {
	float t;
	USplineComponent *spline;
	FVector o, r;
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this->GetOwner();
	SpawnParams.Instigator = Instigator;
	AWallSegment *newWallSegment;
	if (!IsValid(breakingSegment)) {
		return;
	}
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("hiho %s"),*breakingSegment->GetName()));
	switch (NearResult) {
	case 0:
		spline = breakingSegment->Spline;
		o = spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
		r = (spline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::World) - o).GetSafeNormal();
		ProjectOnSegment(o,r,breakLocation,t);

		//DEBUG
		DrawDebugPoint(GetWorld(), breakLocation + CAPSULE_RADIUS*r, 25, FColor::Green, false, 10.0f, 0);
		DrawDebugPoint(GetWorld(), breakLocation - CAPSULE_RADIUS*r, 25, FColor::Blue, false, 10.0f, 0);

		//DEBUG END

		
		newWallSegment = GetWorld()->SpawnActor<AWallSegment>(AWallSegment::StaticClass(), breakLocation+CAPSULE_RADIUS*r, GetActorRotation(), SpawnParams);
		newWallSegment->NextSegment = breakingSegment->NextSegment;
		if (IsValid(newWallSegment->NextSegment)) {
			newWallSegment->NextSegment->PrevSegment = newWallSegment;
		}
		newWallSegment->PrevSegment = NULL;
		newWallSegment->SetBeamColor(InitialWallColor);
		newWallSegment->SetBeamSource(newWallSegment);
		newWallSegment->SetBeamTarget(breakingSegment->NextSegment);
		newWallSegment->SetActorEnableCollision(true);
		newWallSegment->Spline->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		if(IsValid(breakingSegment->NextSegment))
			newWallSegment->UpdateSplineLocation(breakingSegment->NextSegment->GetActorLocation());

		newWallSegment = GetWorld()->SpawnActor<AWallSegment>(AWallSegment::StaticClass(), breakLocation - CAPSULE_RADIUS*r, GetActorRotation(), SpawnParams);
		newWallSegment->NextSegment = NULL;
		newWallSegment->PrevSegment = breakingSegment;
		newWallSegment->WallBeams->DestroyComponent();
		newWallSegment->Spline->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		newWallSegment->Spline->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		breakingSegment->NextSegment = newWallSegment;
		breakingSegment->SetBeamTarget(newWallSegment);
		breakingSegment->SetActorEnableCollision(true);
		breakingSegment->UpdateSplineLocation(newWallSegment->GetActorLocation());
		//TODO Possible Workaround -> Exchange breakingSegment with new one?!
		break;
	case 1:
		break;
	case 2:
		break;
	default:
		break;
	}

}

void AWallSegment::ProjectOnSegment(FVector &o, FVector &r, FVector &p, float &t) {
	t = ((p - o)*r)|r;
	UKismetSystemLibrary::PrintString(GetWorld(), FString::Printf(TEXT("Projecting %s on %s with t = %f."),*p.ToString(),*(o+t*r).ToString(),t));
	p = o + FMath::Abs(t)*r;
}

AWallSegment *AWallSegment::FindSegmentToBreak(bool forwardSearch, FVector breakLocation, int &NearResult) {
	if (IsSegmentNearBreakPoint(this, breakLocation) == 0) {
		NearResult = 0;
		return this;
	}
	AWallSegment *traversingRef = 0;
	if (forwardSearch ? IsValid(NextSegment) : IsValid(PrevSegment)) {
		traversingRef = forwardSearch ? NextSegment : PrevSegment;

		while (forwardSearch ? IsValid(traversingRef->NextSegment) : IsValid(traversingRef->PrevSegment)) {
			traversingRef = forwardSearch ? traversingRef->NextSegment : traversingRef->PrevSegment;
			if (forwardSearch ? IsValid(traversingRef->PrevSegment) : IsValid(traversingRef->NextSegment)) {
				AWallSegment *checkedSegment = forwardSearch ? traversingRef->PrevSegment : traversingRef->NextSegment;
				int result = IsSegmentNearBreakPoint(checkedSegment, breakLocation);
				if (result > 0 && result < 3) {
					NearResult = result;
					return checkedSegment;
				}
			}
		}
		if (IsValid(traversingRef)) {
			NearResult = IsSegmentNearBreakPoint(traversingRef, breakLocation);
			return traversingRef;
		}
	}
	return NULL;
}

int AWallSegment::IsSegmentNearBreakPoint(AWallSegment *segment, FVector &breakLocation) {
	if (!IsValid(segment)) {
		return 0;
	}
	USplineComponent *segmentSpline = segment->Spline;
	FVector start = segmentSpline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);
	FVector end = segmentSpline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::World);
	int IsStartNear = (start - breakLocation).Size() < 2 * CAPSULE_RADIUS ? 1 : 0;
	int IsEndNear = (end - breakLocation).Size() < 2 * CAPSULE_RADIUS ? 2 : 0;
	return (IsStartNear + IsEndNear);//0 - nothing near | 1 - start near | 2 - end near | 3 - both near
}
