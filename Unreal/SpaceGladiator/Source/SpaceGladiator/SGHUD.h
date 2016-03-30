// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "SGHUD.generated.h"

/**
 * 
 */
UCLASS()
class SPACEGLADIATOR_API ASGHUD : public AHUD
{
	GENERATED_BODY()
		
public:
	ASGHUD(const FObjectInitializer &ObjectInitializer);
	void DrawHUD();
	
	UFont *HUDFont;
};
