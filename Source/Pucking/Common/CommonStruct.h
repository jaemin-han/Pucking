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
	EGunType GunType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Magazine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxMagazine;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Recoil;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range;
};

class PUCKING_API CommonStruct
{
public:
	CommonStruct();
	~CommonStruct();
};
