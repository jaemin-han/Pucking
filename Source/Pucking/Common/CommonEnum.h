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
enum class EOptionType : uint8
{
	MaxHP,
	DF,
	Dmg,
	Critical
};

UENUM(BlueprintType)
enum class EItemType : uint8
{
	Ammo UMETA(DisplayName = "Ammo"),
	// todo: 필요 시 추후 추가
};

// Ammo type, rifle, shotgun 등등
UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	Rifle UMETA(DisplayName = "Rifle"),
	Shotgun UMETA(DisplayName = "Shotgun"),
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


/**
 * 
 */
class PUCKING_API CommonEnum
{
public:
	CommonEnum();
	~CommonEnum();
};
