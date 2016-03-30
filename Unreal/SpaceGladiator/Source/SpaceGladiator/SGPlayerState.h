// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "SGPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SPACEGLADIATOR_API ASGPlayerState : public APlayerState
{
	GENERATED_BODY()

public:

		/** Stats */
		UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Stats)
		int32 Kills;
		UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Stats)
		int32 Assists;
		UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Stats)
		int32 Deaths;

};
