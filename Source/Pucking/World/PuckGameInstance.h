// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "LevelData.h"
#include "Blueprint/UserWidget.h"
#include "UI/Status/GameOverUI.h"
#include "UI/Status/GameClearUI.h"
#include "PuckGameInstance.generated.h"

//레벨 변경되면 호출하는 함수
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLevelChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameOver);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameClear);
/**
 * 
 */
UCLASS()
class PUCKING_API UPuckGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	UFUNCTION(BlueprintCallable)
	FSpawnToLevelData GetDataByLevel(FName RowName);
	UFUNCTION(BlueprintCallable)
	void LevelCheck();
	UFUNCTION()
	void DoKillCount();
	UFUNCTION()
	void HalfTimer();
	UFUNCTION()
	void GameOver();

	//Widget Controll
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameOverUI> GameOverUIClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameClearUI> GameClearUIClass;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UGameOverUI* GameOverUI;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	UGameClearUI* GameClearUI;
	UFUNCTION(BlueprintCallable)
	void ShowGameOverWidget();
	UFUNCTION(BlueprintCallable)
	void ShowGameClearWidget();
	UFUNCTION(BlueprintCallable)
	void HideGameOverWidget();

private:
	UPROPERTY()
	UUserWidget* CurrentWidget;


public:
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
	int32 Goal = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 NumberOfEnemy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotalKillCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HalfTimeSecondsGameInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HalfTimeOrigin = 10;
	
	FTimerHandle HalfTimerInGameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsHalf = false;

	//delegate
	UPROPERTY(BlueprintAssignable, Category = "LevelChanged")
	FOnLevelChanged OnLevelChanged;
	UPROPERTY(BlueprintAssignable, Category = "LevelChanged")
	FOnGameOver OnGameOver;
	UPROPERTY(BlueprintAssignable, Category = "LevelChanged")
	FOnGameClear OnGameClear;
	// Enemy Data Asset Array
	UPROPERTY(EditDefaultsOnly, Category = "Option")
	TArray<class UOptionDataAsset*> EnemyOptionDataAssets;
};
