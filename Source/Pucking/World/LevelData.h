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
	float NormalEnemyDamageIncreaseRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NormalGroup")
	float NormalEnemyHPIncreaseRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnWeight")
	float MinionWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnWeight")
	float TankWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnWeight")
	float RangerWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnWeight")
	float NormalWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnWeight")
	float EliteWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EliteGroup")
	float EliteEnemyDamageIncreaseRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EliteGroup")
	float EliteEnemyHPIncreaseRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropRate")
	float NormalEnemyDropRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropRate")
	float EliteEnemyDropRate;
};


class PUCKING_API LevelData
{
public:
	LevelData();
	~LevelData();
};
