// Fill out your copyright notice in the Description page of Project Settings.


#include "World/LevelTestPuckingGameMode.h"
#include "Kismet/GameplayStatics.h"

ALevelTestPuckingGameMode::ALevelTestPuckingGameMode()
{
}

FSpawnToLevelData ALevelTestPuckingGameMode::GetValueData(FName RowName)
{
	FSpawnToLevelData Result;
	if (!LevelSpawnDataTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("DataTable is not Assigned!"));
		return Result;
	}

	FSpawnToLevelData* Row = LevelSpawnDataTable->FindRow<FSpawnToLevelData>(RowName, TEXT(""));
	if (Row)
	{
		Result = *Row;
		UE_LOG(LogTemp, Warning, TEXT("$$$$$$$$$$$$$$ \n %s >> \n Increase [Damage : %f, HP : %f] \n Count [Nothing : %d, Tank : %d, Range : %d] \n SpawnDelay [%f] \n $$$$$$$$$$$$$$$$$$$$"),
			*RowName.ToString(), Row->NormalDamageIncreaseRate, Row->NormalHPIncreaseRate, Row->NormalNothingCount, Row->NormalTankCount, Row->NormalRangeCount, Row->NormalGroupSpawnDelay);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Row %s not Found in DataTable"), *RowName.ToString());
	}
	
	return Result;
}

void ALevelTestPuckingGameMode::SetEnemy(FName LevelName)
{
	FLatentActionInfo LatentInfo;
	LatentInfo.CallbackTarget = this;
	LatentInfo.ExecutionFunction = FName("LevelStreamMessage");
	LatentInfo.Linkage = 0;
	LatentInfo.UUID = __LINE__;
	UGameplayStatics::LoadStreamLevel(this, LevelName, true, false, LatentInfo);

}

void ALevelTestPuckingGameMode::UnloadLevel(FName LevelName)
{
	UGameplayStatics::UnloadStreamLevel(this, LevelName, FLatentActionInfo(), true);
}

void ALevelTestPuckingGameMode::LevelCheck()
{
	switch (LevelCount)
	{
	case 1:
		LevelRowName = TEXT("FirstLevel");
		GetValueData(LevelRowName);
		
		break;
	case 2:
		LevelRowName = TEXT("SecondLevel");
		GetValueData(LevelRowName);
		Lv_StreamingLevel = TEXT("Lv_StreamingLevel");
		SetEnemy(Lv_StreamingLevel);
		break;
	case 3:
		LevelRowName = TEXT("ThirdLevel");
		GetValueData(LevelRowName);
		UnloadLevel(Lv_StreamingLevel);
		break;
	case 4:
		LevelRowName = TEXT("FourthLevel");
		GetValueData(LevelRowName);
		break;
	case 5:
		LevelRowName = TEXT("FifthLevel");
		GetValueData(LevelRowName);
		break;
	default:
		break;
	}
	if (LevelCount >= 5)
	{
		LevelCount = 1;
	}
	else
	{
		LevelCount++;
	}

}

void ALevelTestPuckingGameMode::LevelStreamMessage()
{
	UE_LOG(LogTemp, Warning, TEXT("LevelStreaming Success"));
}

