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

// Ammo Damage type, physical, fire, ice 등등
UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Physical UMETA(DisplayName = "Physical"),
	Fire UMETA(DisplayName = "Fire"),
	Ice UMETA(DisplayName = "Ice"),
};

// Ammo type, rifle, shotgun 등등
UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	Rifle UMETA(DisplayName = "Rifle"),
	Shotgun UMETA(DisplayName = "Shotgun"),
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


#pragma region ItemType::Ammo
	
	// item type 이 Ammo 일 경우, 잔탄량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (EditCondition = "ItemType == EItemType::Ammo"))
	int32 AmmoCount;
	
	// item type 이 Ammo 일 경우, EDamageType
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (EditCondition = "ItemType == EItemType::Ammo"))
	EDamageType DamageType;

	// item type 이 Ammo 일 경우, EAmmoType
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (EditCondition = "ItemType == EItemType::Ammo"))
	EAmmoType AmmoType;

	//  item type 이 Ammo 일 경우, AmmoDamage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (EditCondition = "ItemType == EItemType::Ammo"))
	float AmmoDamage;

	// item type 이 Ammo 일 경우, 치명타 확률
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (EditCondition = "ItemType == EItemType::Ammo"))
	float CriticalRate;

	// item type 이 Ammo 일 경우, 치명타 배율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (EditCondition = "ItemType == EItemType::Ammo"))
	float CriticalMultiplier;

#pragma endregion

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
