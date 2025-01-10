// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Physical UMETA(DisplayName = "Physical"),
	Fire UMETA(DisplayName = "Fire"),
	Ice UMETA(DisplayName = "Ice"),
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	// 결정되지 않음
	Empty UMETA(DisplayName = "Empty"),
	
	// Pickable
	Ammo UMETA(DisplayName = "Ammo"),

	// Overlap
	Essence UMETA(DisplayName = "Essence"),
	HealthMarble UMETA(DisplayName = "HealthMarble"),
};

// Ammo type, rifle, shotgun 등등
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Rifle UMETA(DisplayName = "Rifle"),
	Shotgun UMETA(DisplayName = "Shotgun"),
	WeaponTBD1 UMETA(DisplayName = "WeaponTBD1"),
	WeaponTBD2 UMETA(DisplayName = "WeaponTBD2"),
};

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

UENUM(BlueprintType)
enum class EOptionType : uint8
{
#pragma region Ammo Default Options
	DamageType UMETA(DisplayName = "DamageType, don't use this"),
	Damage UMETA(DisplayName = "Damage, don't use this"),
#pragma endregion
	CriticalRate UMETA(DisplayName = "CriticalRate"),
	CriticalMultiplier UMETA(DisplayName = "CriticalMultiplier"),
	PhysicalPenetration UMETA(DisplayName = "PhysicalPenetration"),
	FirePenetration UMETA(DisplayName = "FirePenetration"),
	IcePenetration UMETA(DisplayName = "IcePenetration"),

	// todo: 이 부분은 호환성을 위해 남겨둡니다. 삭제해주시길 바랍니다.
#pragma region old option
	MaxHP,
	DF,
	Dmg,
	Critical
#pragma endregion
};


/**
 * 
 */
class PUCKING_API CommonEnum
{
public:
	CommonEnum();
	~CommonEnum();
};
