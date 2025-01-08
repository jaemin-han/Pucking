// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "LevelData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FSpawnToLevelData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NormalGroup")
	float NormalDamageIncreaseRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NormalGroup")
	float NormalHPIncreaseRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NormalGroup")
	int32 NormalNothingCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NormalGroup")
	int32 NormalTankCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NormalGroup")
	int32 NormalRangeCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NormalGroup")
	float NormalGroupSpawnDelay;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EliteGroup")
	float EliteDamageIncreaseRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EliteGroup")
	float EliteHPIncreaseRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EliteGroup")
	int32 EliteNothingCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EliteGroup")
	int32 EliteTankCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EliteGroup")
	int32 EliteRangeCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EliteGroup")
	float EliteGroupSpawnDelay;
};


class PUCKING_API LevelData
{
public:
	LevelData();
	~LevelData();
};
