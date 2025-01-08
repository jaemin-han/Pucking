// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "World/PuckingGameMode.h"
#include "Engine/DataTable.h"
#include "LevelData.h"
#include "LevelTestPuckingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API ALevelTestPuckingGameMode : public APuckingGameMode
{
	GENERATED_BODY()
	
public:
	ALevelTestPuckingGameMode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* LevelSpawnDataTable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LevelCount = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName LevelRowName;
	UFUNCTION(BlueprintCallable)
	FSpawnToLevelData GetValueData(FName RowName);

	UFUNCTION(BlueprintCallable)
	void SetEnemy(FName LevelName);

	UFUNCTION(BlueprintCallable)
	void UnloadLevel(FName LevelName);
	UFUNCTION(BlueprintCallable)
	void LevelCheck();

	UFUNCTION(BlueprintCallable)
	void LevelStreamMessage();

	UPROPERTY(EditAnywhere, Category = "Lv_StreamingLevel")
	FName Lv_StreamingLevel;
};
