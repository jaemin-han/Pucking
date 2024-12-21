// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CommonEnum.h"
#include "ItemDropData.generated.h"

/**
 * 
 */
/**
 * Ammo-specific data structure
 */
USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_BODY()

	// Ammo count
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 AmmoCount;

	// Damage type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	EDamageType DamageType;

	// Ammo type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	EAmmoType AmmoType;

	// Ammo damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float AmmoDamage;

	// Critical hit rate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float CriticalRate;

	// Critical hit multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float CriticalMultiplier;
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


	// FAmmoData
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FAmmoData AmmoData;

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
