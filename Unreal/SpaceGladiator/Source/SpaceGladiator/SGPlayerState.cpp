// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "SGPlayerState.h"
#include "UnrealNetwork.h"


void ASGPlayerState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate to everyone
	DOREPLIFETIME(ASGPlayerState, Kills);
	DOREPLIFETIME(ASGPlayerState, Assists);
	DOREPLIFETIME(ASGPlayerState, Deaths);
	DOREPLIFETIME(ASGPlayerState, Color);
}