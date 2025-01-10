// Fill out your copyright notice in the Description page of Project Settings.


#include "World/LevelTestPuckingGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "ActorComponent/EnemyStatusComponent.h"

ALevelTestPuckingGameMode::ALevelTestPuckingGameMode()
{
}

FSpawnToLevelData ALevelTestPuckingGameMode::GetValueData(FName RowName)
{
	
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
	
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), FoundActors);
		for (AActor* Actor : FoundActors)
		{
			AllEnemy = Actor->FindComponentByClass<UEnemyStatusComponent>();
			if (AllEnemy)
			{
				AllEnemy->RemainHP = AllEnemy->RemainHP + Row->NormalHPIncreaseRate;
				AllEnemy->EnemyDamageToLevel = AllEnemy->EnemyDamageToLevel + Row->NormalDamageIncreaseRate;
			}
		}
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
	//Set StreamingLevel Name
	Lv_StreamingLevel = TEXT("Lv_StreamingLevel");
	switch (LevelCount)
	{
	case 1:
		//UnloadLevel(Lv_StreamingLevel);
		LevelRowName = TEXT("FirstLevel");
		
		//Streaming Level
		SetEnemy(Lv_StreamingLevel);

		//Find RowName And RowName's Data
		GetValueData(LevelRowName);
		
		break;
	case 2:
		//Unload Level
		UnloadLevel(Lv_StreamingLevel);

		LevelRowName = TEXT("SecondLevel");
		SetEnemy(Lv_StreamingLevel);
		GetValueData(LevelRowName);
		//Streaming Level
		break;
	case 3:
		UnloadLevel(Lv_StreamingLevel);

		LevelRowName = TEXT("ThirdLevel");
		SetEnemy(Lv_StreamingLevel);
		GetValueData(LevelRowName);

		break;
	case 4:
		UnloadLevel(Lv_StreamingLevel);

		LevelRowName = TEXT("FourthLevel");
		SetEnemy(Lv_StreamingLevel);
		GetValueData(LevelRowName);

		break;
	case 5:
		UnloadLevel(Lv_StreamingLevel);

		LevelRowName = TEXT("FifthLevel");
		SetEnemy(Lv_StreamingLevel);
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

