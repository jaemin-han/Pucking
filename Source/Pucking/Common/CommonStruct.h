// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonEnum.h"
#include "CommonStruct.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FShotgunInfo : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType GunType = EWeaponType::Rifle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Magazine = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxMagazine = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultDamage = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoilX = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoilY = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoilZ = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range = 0;
};

class PUCKING_API CommonStruct
{
public:
	CommonStruct();
	~CommonStruct();
};
