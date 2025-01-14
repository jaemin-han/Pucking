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

	if (LevelNum == 1)
	{
		LvString = TEXT("Level1");
		LevelNum++;
	}
	else if (LevelNum == 2)
	{
		LvString = TEXT("Level2");
		LevelNum++;
	}
	else if (LevelNum == 3)
	{
		LvString = TEXT("Level3");
		LevelNum++;
	}
	else if (LevelNum == 4)
	{
		LvString = TEXT("Level4");
		LevelNum++;
	}
	else if (LevelNum == 5)
	{
		LvString = TEXT("Level5");
		LevelNum++;
	}
	else
	{
		LevelNum = 1;
		//GetWorld()->GetTimerManager().ClearTimer(LevelHandle);
	}


	CurrentRow = GetDataByLevel(LvString);
	

	UE_LOG(LogTemp, Warning, TEXT("NormalEnemyHPIncreaseRate == %f"), CurrentRow.NormalEnemyHPIncreaseRate);
	//GetWorld()->GetTimerManager().SetTimer(LevelHandle, this, &UPuckGameInstance::LevelCheck, 8, false);
}
