#pragma once

UENUM(BlueprintType)
enum class EItemType : uint8
{
	ItemType_None				UMETA(DisplayName = "Not Set"),
	ItemType_InvertControls		UMETA(DisplayName = "Invert Controls"),
	ItemType_Magnet			 	UMETA(DisplayName = "Magnet"),
	ItemType_Detonator			UMETA(DisplayName = "Detonator"),
};

// This needs to be kept in sync with the ItemType Enum
static FLinearColor ItemColors[] = {
	{ 0.0f, 0.0f , 0.0f, 0.0f },    // None
	{ 1.0f, 0.25f, 0.0f, 1.0f },    // Invert
	{ 1.0f, 0.0f , 0.6f, 1.0f },    // Magnet 
	{ 0.0f, 0.5f , 1.0f, 1.0f },    // Detonator
};

// This needs to be kept in sync with the ItemType Enum
static TCHAR* ItemTextures[] = {
	TEXT("Texture2D'/Engine/EditorLandscapeResources/WhiteSquareTexture.WhiteSquareTexture'"),
	TEXT("Texture2D'/Game/Materials/ItemMaterials/ItemBomb.ItemBomb'"),
	TEXT("Texture2D'/Game/Materials/ItemMaterials/ItemMagnet.ItemMagnet'"),
	TEXT("Texture2D'/Game/Materials/ItemMaterials/ItemDetonator.ItemDetonator'"),
};

