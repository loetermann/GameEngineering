// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "Kismet/GameplayStatics.h"
#include "SGCharacter.h"
#include "SGHUD.h"


#define HEALTH_BAR_X_OFFSET 0
#define HEALTH_BAR_Y_OFFSET 20
#define PLATE_Z_OFFSET 300
#define HEALTH_BAR_HEIGHT 10
#define HEALTH_BAR_WIDTH 100


ASGHUD::ASGHUD(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer) {
	static ConstructorHelpers::FObjectFinder<UFont> FontFinder(TEXT("Font'/Engine/EngineFonts/Roboto.Roboto'"));
	if (FontFinder.Object) {
		HUDFont = FontFinder.Object;
	}
}

void ASGHUD::DrawHUD() {
	Super::DrawHUD();
	TArray<AActor*> allPawns;
	UGameplayStatics::GetAllActorsOfClass(this, ASGCharacter::StaticClass(), allPawns);
	ASGCharacter *castedPawn;

	ASGCharacter *ownedPawn = Cast<ASGCharacter>(GetOwningPlayerController()->GetPawn());
	for (AActor* pawn : allPawns) {
		castedPawn = Cast<ASGCharacter>(pawn);
		if (IsValid(castedPawn)) {
			FVector Position = castedPawn->GetActorLocation() + FVector(0, 0, PLATE_Z_OFFSET);
			FVector2D ScreenPosition;
			bool IsOnScreen = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayerController(), Position, ScreenPosition);
			if (IsOnScreen && castedPawn != ownedPawn) {
				DrawText(ownedPawn->GetName(), FLinearColor(1, 1, 1, 0.25f), ScreenPosition.X, ScreenPosition.Y);
				//Background of Health-Bar
				DrawRect(FLinearColor(0.74f, 0.74f, 0.74f, 0.25f), ScreenPosition.X + HEALTH_BAR_X_OFFSET, ScreenPosition.Y + HEALTH_BAR_Y_OFFSET, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT);
				//Fill Health Bar
				float WidthScale = castedPawn->Health / 100.0f;
				DrawRect(FLinearColor(0, 0.54f, 0, 0.25f), ScreenPosition.X + HEALTH_BAR_X_OFFSET + 1, ScreenPosition.Y + HEALTH_BAR_Y_OFFSET + 1, WidthScale * (HEALTH_BAR_WIDTH-2), HEALTH_BAR_HEIGHT - 2);
			}
		}
	}
}