// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "LevelData.h"
#include "PuckGameInstance.generated.h"

//레벨 변경되면 호출하는 함수
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelChanged);
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
	UFUNCTION()
	void DoKillCount();
	UFUNCTION()
	void HalfTimer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* DataByLevel;

	UPROPERTY(EditAnywhere)
	FSpawnToLevelData CurrentRow;

	UPROPERTY()
	FName LvString;

	//UPROPERTY()
	//FTimerHandle LevelHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 LevelNum = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 KillCount = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Goal = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HalfTimeSecondsGameInstance = 30;
	
	FTimerHandle HalfTimerInGameInstance;


	bool bIsHalf = false;

	//delegate
	UPROPERTY(BlueprintAssignable, Category = "LevelChanged")
	FOnLevelChanged OnLevelChanged;

	// Enemy Data Asset Array
	UPROPERTY(EditDefaultsOnly, Category = "Option")
	TArray<class UOptionDataAsset*> EnemyOptionDataAssets;
};
