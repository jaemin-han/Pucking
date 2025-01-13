// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "LevelData.h"
#include "PuckGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API UPuckGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	FSpawnToLevelData GetDataByLevel(FName RowName);
	UFUNCTION(BlueprintCallable)
	void LevelCheck();



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* DataByLevel;
	UPROPERTY(EditAnywhere)
	float CurrentNormalEnemyDamageIncreaseRate;
	UPROPERTY(EditAnywhere)
	float CurrentNormalEnemyHPIncreaseRate;

	UPROPERTY(EditAnywhere)
	float CurrentMinionWeight;
	UPROPERTY(EditAnywhere)
	float CurrentTankWeight;
	UPROPERTY(EditAnywhere)
	float CurrentRangerWeight;

	UPROPERTY(EditAnywhere)
	float CurrentNormalWeight;
	UPROPERTY(EditAnywhere)
	float CurrentEliteWeight;

	UPROPERTY(EditAnywhere)
	float CurrentEliteEnemyDamageIncreaseRate;
	UPROPERTY(EditAnywhere)
	float CurrentEliteEnemyHPIncreaseRate;

	UPROPERTY(EditAnywhere)
	float CurrentNormalEnemyDropRate;
	UPROPERTY(EditAnywhere)
	float CurrentEliteEnemyDropRate;

	UPROPERTY()
	FName LvString;
};
