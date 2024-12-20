// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Physical,
	Fire,
	Ice
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
enum class EGunType : uint8
{
	Shotgun,
	Rifle,
	Grenade
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
