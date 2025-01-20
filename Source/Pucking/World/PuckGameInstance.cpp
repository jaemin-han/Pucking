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
	}
	else if (LevelNum == 2)
	{
		LvString = TEXT("Level2");
	}
	else if (LevelNum == 3)
	{
		LvString = TEXT("Level3");
	}
	else if (LevelNum == 4)
	{
		LvString = TEXT("Level4");
	}
	else if (LevelNum == 5)
	{
		LvString = TEXT("Level5");
	}
	else
	{
		LevelNum = 1;
		LevelCheck();
		return;
		//GetWorld()->GetTimerManager().ClearTimer(LevelHandle);
	}


	UE_LOG(LogTemp, Warning, TEXT("PuckGameInstance::LEVELCHECK::CurrentLevel <<%d>>"), LevelNum);
	CurrentRow = GetDataByLevel(LvString);
	
	//레벨이 변경되었다고 알림. 레벨변경 시 동작해야 하는 곳에서 받기(EnemySpawnerTest 등)
	OnLevelChanged.Broadcast();


	//NextLevel
	LevelNum++;

	//GetWorld()->GetTimerManager().SetTimer(LevelHandle, this, &UPuckGameInstance::LevelCheck, 8, false);
}

void UPuckGameInstance::DoKillCount()
{
	if (KillCount >= 5)
	{
		LevelCheck();
		KillCount = 0;
	}
	KillCount++;
	UE_LOG(LogTemp, Warning, TEXT("PuckGameInstance::DoKillCount %d"), KillCount);
}
