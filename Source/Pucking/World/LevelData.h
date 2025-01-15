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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDrop")
	float NormalEnemyDropMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDrop")
	float EliteEnemyDropMultiplier;

	// 몬스터의 희귀도 배율
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDrop")
	float NormalRarityMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDrop")
	float EliteRarityMultiplier;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDrop")
	int32 ItemTier;
	// Enemy 에 적용될 옵션 티어
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDrop")
	int32 EnemyOptionTier;
};


class PUCKING_API LevelData
{
public:
	LevelData();
	~LevelData();
};
