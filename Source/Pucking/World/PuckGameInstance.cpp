// Fill out your copyright notice in the Description page of Project Settings.


#include "World/PuckGameInstance.h"
#include "Engine/DataTable.h"

FSpawnToLevelData UPuckGameInstance::GetDataByLevel(FName RowName)
{

	if (!DataByLevel)
	{
		return FSpawnToLevelData();
	}

	FSpawnToLevelData* Row = DataByLevel->FindRow<FSpawnToLevelData>(RowName, "");

	if (Row)
	{
		return *Row;
	}

	return FSpawnToLevelData();
}

void UPuckGameInstance::LevelCheck()
{
	LvString = TEXT("Level1");

	CurrentNormalEnemyHPIncreaseRate = GetDataByLevel(LvString).NormalEnemyHPIncreaseRate;
	UE_LOG(LogTemp, Warning, TEXT("NormalEnemyDamageIncreaseRate == %f"), CurrentNormalEnemyHPIncreaseRate);
}
