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
	FSpawnToLevelData CurrentRow;

	UPROPERTY()
	FName LvString;

	//UPROPERTY()
	//FTimerHandle LevelHandle;

	UPROPERTY()
	float LevelNum = 1;
};
