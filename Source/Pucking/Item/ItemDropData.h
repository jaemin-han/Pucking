// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDropData.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Ammo UMETA(DisplayName = "Ammo"),
	// todo: 필요 시 추후 추가
};



USTRUCT(BlueprintType)
struct FItemDropData : public FTableRowBase
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

	// item thumbnail
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UTexture2D* ItemThumbnail;

	// item drop rate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ItemDropRate;

	// item rarity rate multiplier: normal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float NormalRarityRate;

	// item rarity rate multiplier: magic
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float MagicRarityRate;

	// item rarity rate multiplier: rare
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float RareRarityRate;

	// stackable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bStackable;

	// max stack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (EditCondition = "bStackable"))
	int32 MaxStackCount;
};

class PUCKING_API ItemDropData
{
public:
	ItemDropData();
	~ItemDropData();
};
