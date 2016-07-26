#pragma once

UENUM(BlueprintType)
enum class EItemType : uint8
{
	ItemType_None				UMETA(DisplayName = "Not Set"),
	ItemType_InvertControls		UMETA(DisplayName = "Invert Controls"),
	ItemType_InvertCamera		UMETA(DisplayName = "Invert Camera"),
	ItemType_Magnet			 	UMETA(DisplayName = "Magnet"),
	ItemType_ProjectileAbsorb	UMETA(DisplayName = "Absorb Projectiles"),
	ItemType_Unstoppable		UMETA(DisplayName = "Unstoppable")
};

// This needs to be kept in sync with the ItemType Enum
static FLinearColor ItemColors[] = {
	{ 0.00f, 0.00f, 0.00f, 0.0f },    // None
	{ 0.00f, 1.00f, 0.00f, 1.0f },    // Controls
	{ 1.00f, 0.66f, 0.00f, 1.0f },    // Camera
	{ 1.00f, 0.00f, 0.66f, 1.0f },    // Magnet 
	{ 0.00f, 0.66f, 1.00f, 1.0f },    // Absorb
	{ 0.00f, 0.00f, 1.00f, 1.0f },    // Unstoppable
};

// This needs to be kept in sync with the ItemType Enum
static TCHAR* ItemGlowTextures[] = {
	TEXT("Texture2D'/Engine/EditorLandscapeResources/WhiteSquareTexture.WhiteSquareTexture'"),
	TEXT("Texture2D'/Game/Materials/ItemMaterials/ItemInvertControls.ItemInvertControls'"),
	TEXT("Texture2D'/Game/Materials/ItemMaterials/ItemInvertCamera.ItemInvertCamera'"),
	TEXT("Texture2D'/Game/Materials/ItemMaterials/ItemMagnet.ItemMagnet'"),
	TEXT("Texture2D'/Game/Materials/ItemMaterials/ItemAbsorb.ItemAbsorb'"),
	TEXT("Texture2D'/Game/Materials/ItemMaterials/ItemUnstoppable.ItemUnstoppable'"),
};

static TCHAR* ItemIcons[] = {
	TEXT("Texture2D'/Engine/EditorLandscapeResources/WhiteSquareTexture.WhiteSquareTexture'"),
	TEXT("Texture2D'/Game/Items/InvertControls.InvertControls'"),
	TEXT("Texture2D'/Game/Items/InvertCamera.InvertCamera'"),
	TEXT("Texture2D'/Game/Items/Magnet.Magnet'"),
	// TODO: Replace with new icons
	TEXT("Texture2D'/Game/Items/Absorb.Absorb'"),
	TEXT("Texture2D'/Game/Items/Unstoppable.Unstoppable'"),
};
