// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "DrawDebugHelpers.h"
#include "ItemType.h"
#include "ItemSpawnPoint.h"
#include "SGBlueprintUtilities.h"

FVector USGBlueprintUtilities::CalculateAIAiming(APawn *shooter, APawn *target, float projectileVelocity) {
	FVector s = shooter->GetActorLocation();
	FVector v = target->GetActorForwardVector() * target->GetMovementComponent()->GetMaxSpeed();
	FVector t = target->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("Input:\n s = %s\n v = %s\n t = %s\nvel = %f\n"), *s.ToString(), *v.ToString(), *t.ToString(),projectileVelocity);
	double p = 2.0*t.X * v.X - 2.0*v.X * s.X
		         + 2.0*t.Y * v.Y - 2.0*v.Y * s.Y;
	double c = FMath::Pow(v.X, 2.0f) + FMath::Pow(v.Y, 2.0f) - FMath::Pow(projectileVelocity,2.0f);
	double pHalf = p / (2.0*c);

	double q = FMath::Pow(t.X, 2.0f) + FMath::Pow(t.Y, 2.0f) - 2 * s.X*t.X - 2 * s.Y*t.Y + FMath::Pow(s.X, 2.0f) + FMath::Pow(s.Y, 2.0f);
	q /= c;
	

	//UE_LOG(LogTemp, Warning, TEXT("f(x)=x^2+%f*x+%f\nc = %f"),p/c,q);
	//UE_LOG(LogTemp, Warning, TEXT("f(x) = sqrt((%f+x*%f-%f)^2+(%f+x*%f-%f)^2)"), t.X, v.X, s.X, t.Y, v.Y, s.Y);
	//UE_LOG(LogTemp, Warning, TEXT("g(x) = x * %f"), projectileVelocity);

	double temp = (FMath::Pow(pHalf, 2.0f) - q);
	//UE_LOG(LogTemp, Warning, TEXT("temp = %f\nsqrt(temp) = %f"), temp,FMath::Sqrt(temp));
	double t2 = FMath::Sqrt(temp) - pHalf;
	//UE_LOG(LogTemp, Warning, TEXT("t2 = %f"), t2);
	if (isnan(t2)) {
		t2 = 1.0f;
	}
	//UE_LOG(LogTemp, Warning, TEXT("pHalf = %f, q = %f, c = %f, t2 = %f\n"), pHalf, q, c, t2);
	FVector result = (t + t2*v) - s;
	result.Normalize();
	//DrawDebugPoint(shooter->GetWorld(), t+v*t2, 150, FColor(0, 0, 255),false,0.2f);
	//result = result.RotateAngleAxis(90.0f, FVector(0, 0, 1));
	return result;
}

float USGBlueprintUtilities::GetMaxProjectileSpeed() {
	return 10000.0f;
}

void USGBlueprintUtilities::SpawnItem(UObject *context, EItemType type)
{
	/*
	TActorIterator<AItemSpawnPoint> ActorItr(GetWorld());
	for (TActorIterator<AItemSpawnPoint> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		
	}
	*/
	if (IsValid(context)) {
		TArray<AActor*> spawns;
		UGameplayStatics::GetAllActorsOfClass(context, AItemSpawnPoint::StaticClass(), spawns);
		if (spawns.Num() > 0) {
			int32 spawnIndex = FMath::RandRange(0, spawns.Num()-1);
			AItemSpawnPoint *spawnPoint = (AItemSpawnPoint*)spawns[spawnIndex];
			if (IsValid(spawnPoint)) {
				spawnPoint->SpawnItem(type);
			}
		}
	}
}

