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
	BFG UMETA(DisplayName = "BFG"),
	Hammer UMETA(DisplayName = "Hammer"),
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
	PhysicalDefense UMETA(DisplayName = "PhysicalDefense"),
	FireDefense UMETA(DisplayName = "FireDefense"),
	IceDefense UMETA(DisplayName = "IceDefense"),
	StaggerValue UMETA(DisplayName = "StaggerValue"),
};

UENUM(BlueprintType)
enum class ECharacterMontage : uint8
{
	RifleFire UMETA(DisplayName = "RifleFire"),
	RifleZoomFire UMETA(DisplayName = "RifleZoomFire"),
	RifleReload UMETA(DisplayName = "RifleReload"),
	ShotgunFire UMETA(DisplayName = "ShotgunFire"),
	ShotgunZoomFire UMETA(DisplayName = "ShotgunZoomFire"),
	ShotgunReload UMETA(DisplayName = "ShotgunReload"),
	BFGFire UMETA(DisplayName = "BFGFire"),
	BFGZoomFire UMETA(DisplayName = "BFGZoomFire"),
	BFGReload UMETA(DisplayName = "BFGReload"),
	Switching UMETA(DisplayName = "Switching"),
	HookMode UMETA(DisplayName = "HookMode"),
	Hooking UMETA(DisplayName = "Hooking"),
	HookStart UMETA(DisplayName = "HookStart"),
	JetpackMode UMETA(DisplayName = "JetpackMode")
};

UENUM(BlueprintType)
enum class ECharacterFSM : uint8
{
	Idle UMETA(DisplayName = "Idle"),
	Moving UMETA(DisplayName = "Moving"),
	Falling UMETA(DisplayName = "Falling"),
	Fire UMETA(DisplayName = "Fire"),
	Reloading UMETA(DisplayName = "Reloading"),
	Zoom UMETA(DisplayName = "Zoom"),
	Switching UMETA(DisplayName = "Switching"),
	HookMode UMETA(DisplayName = "HookMode"),
	//Hooking UMETA(DisplayName = "Hooking"),
	JetpackMode UMETA(DisplayName = "JetpackMode")
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
