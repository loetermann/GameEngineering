// Fill out your copyright notice in the Description page of Project Settings.

#include "SpaceGladiator.h"
#include "Kismet/GameplayStatics.h"
#include "SGCharacter.h"
#include "SGHUD.h"
#include "ItemType.h"


#define HEALTH_BAR_X_OFFSET 0
#define HEALTH_BAR_Y_OFFSET 20
#define PLATE_Z_OFFSET 300
#define HEALTH_BAR_HEIGHT 10
#define HEALTH_BAR_WIDTH 100
#define ITEM_ICON_DIM 80
#define ITEM_ICON_HALF_DIM 40
#define ITEM_ICON_Y_OFFSET 120



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
	int32 Width, Height;
	GetOwningPlayerController()->GetViewportSize(Width, Height);

	//Show Current Item
	if (ownedPawn->HasItem()) {
		UTexture2D* ItemBitmap = Cast<UTexture2D>(StaticLoadObject(UTexture2D::StaticClass(), NULL, ItemIcons[(uint8)ownedPawn->ItemType]));

		if (!ItemBitmap) { GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor(1.0, 1.0, 1.0), "No Item Bitmap Loaded"); }
		DrawTexture(ItemBitmap, Width/2 - ITEM_ICON_HALF_DIM, Height - ITEM_ICON_Y_OFFSET, ITEM_ICON_DIM, ITEM_ICON_DIM, 
					0, 0, 1, 1, FLinearColor::Blue);
	}

	for (AActor* pawn : allPawns) {
		castedPawn = Cast<ASGCharacter>(pawn);
		if (IsValid(castedPawn)) {
			FVector Position = castedPawn->GetActorLocation() + FVector(0, 0, PLATE_Z_OFFSET);
			FVector2D ScreenPosition;
			bool IsOnScreen = UGameplayStatics::ProjectWorldToScreen(GetOwningPlayerController(), Position, ScreenPosition);
			if (IsOnScreen && castedPawn != ownedPawn) {
				DrawText(castedPawn->GetName(), FLinearColor(1, 1, 1, 0.25f), ScreenPosition.X, ScreenPosition.Y);
				//Background of Health-Bar
				DrawRect(FLinearColor(0.74f, 0.74f, 0.74f, 0.25f), ScreenPosition.X + HEALTH_BAR_X_OFFSET, ScreenPosition.Y + HEALTH_BAR_Y_OFFSET, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT);
				//Fill Health Bar
				float WidthScale = castedPawn->Health / 100.0f;
				DrawRect(FLinearColor(0, 0.54f, 0, 0.25f), ScreenPosition.X + HEALTH_BAR_X_OFFSET + 1, ScreenPosition.Y + HEALTH_BAR_Y_OFFSET + 1, WidthScale * (HEALTH_BAR_WIDTH-2), HEALTH_BAR_HEIGHT - 2);
			}
		}
	}
}