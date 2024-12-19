// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDropData.h"
#include "ItemInstanceData.generated.h"


// item rarity enum class
UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	// Normal
	Normal UMETA(DisplayName = "Normal"),
	// Magic
	Magic UMETA(DisplayName = "Magic"),
	// Rare
	Rare UMETA(DisplayName = "Rare"),
};

USTRUCT(BlueprintType)
struct FItemInstanceData : public FTableRowBase
{
	GENERATED_BODY()

	// item name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemName;

	// static mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UStaticMesh* ItemStaticMesh;

	// skeletal mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class USkeletalMesh* ItemSkeletalMesh;

	// item type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType;

	// item rarity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemRarity ItemRarity;

	// item thumbnail
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UTexture2D* ItemThumbnail;

	// stackable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bStackable;

	// max stack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (EditCondition = "bStackable"))
	int32 MaxStackCount;

	// Item Option Array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<class UItemOption*> ItemOptions;
};

/**
 * 
 */
class PUCKING_API ItemInstanceData
{
public:
	ItemInstanceData();
	~ItemInstanceData();
};
