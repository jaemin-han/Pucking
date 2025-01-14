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

// 직접 주워서 아이템으로 인벤토리에 들어가는 아이템
USTRUCT()
struct FPickableData
{
	GENERATED_BODY()
};

USTRUCT(BlueprintType)
struct FAmmoData : public FPickableData
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
	EWeaponType WeaponType;

	// Ammo damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float AmmoDamage;

	// Critical hit rate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float CriticalRate;

	// Critical hit multiplier
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float CriticalMultiplier;

	// Stagger Value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	float StaggerValue;
};

// 근처에 가기만 하면, Overlap 이 되면 획득되는 아이템 (아이템으로서 적용되지 않음)
USTRUCT(BlueprintType)
struct FOverlapData
{
	GENERATED_BODY()

	// 얼마만큼 겹쳤을 때 획득되는지, 단위 m
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlap")
	float OverlapRadius;

	// 획득이 가능한 시간, 0 이면 무한, 단위 s
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Overlap")
	float LifeTime;
};

// 정수
USTRUCT(BlueprintType)
struct FEssenceData : public FOverlapData
{
	GENERATED_BODY()

	// essence count
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Essence")
	int32 EssenceCount;
};


// 체력 구슬
USTRUCT(BlueprintType)
struct FHealthMarbleData : public FOverlapData
{
	GENERATED_BODY()

	// 체력 회복 량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HealthMarble")
	float HealthRecovery;
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

	// item tier, 0 은 설정되지 않은 것, 1 이 가장 낮고, 숫자가 커질수록 높은 티어
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	int32 ItemTier;

#pragma region Item Type Specific Data
	// FAmmoData
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item",
		meta = (EditCondition = "ItemType == EItemType::Ammo"))
	FAmmoData AmmoData;

	// FEssenceData
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item",
		meta = (EditCondition = "ItemType == EItemType::Essence"))
	FEssenceData EssenceData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item",
		meta = (EditCondition = "ItemType == EItemType::HealthMarble"))
	FHealthMarbleData HealthMarbleData;

#pragma endregion
	// item thumbnail
	UPROPERTY
	(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UTexture2D* ItemThumbnail;

	// item drop rate
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float ItemDropRate;

	// item rarity Weight: normal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float NormalWeight;

	// item rarity Weight: magic
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float MagicWeight;

	// item rarity Weight: rare
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	float RareWeight;

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
