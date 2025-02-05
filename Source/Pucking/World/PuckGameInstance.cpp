// Fill out your copyright notice in the Description page of Project Settings.


#include "World/PuckGameInstance.h"
#include "Engine/DataTable.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Status/GameClearUI.h"


void UPuckGameInstance::Init()
{
	Super::Init();
	HalfTimeSecondsGameInstance = HalfTimeOrigin;
	TotalKillCount = 0;
}

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
		Goal = LevelNum * 10;
	}
	else if (LevelNum == 2)
	{
		LvString = TEXT("Level2");
		Goal = LevelNum * 10;
	}
	else if (LevelNum == 3)
	{
		LvString = TEXT("Level3");
		Goal = LevelNum * 10;
	}
	else if (LevelNum == 4)
	{
		LvString = TEXT("Level4");
		Goal = LevelNum * 10;
	}
	else if (LevelNum == 5)
	{
		LvString = TEXT("Level5");
		Goal = LevelNum * 10;
	}
	else
	{
		//GameClearUI
		OnGameClear.Broadcast();
		ShowGameClearWidget();
		LevelNum = 1;
		return;
		//GetWorld()->GetTimerManager().ClearTimer(LevelHandle);
	}


	//UE_LOG(LogTemp, Warning, TEXT("PuckGameInstance::LEVELCHECK::CurrentLevel <<%d>>"), LevelNum);
	CurrentRow = GetDataByLevel(LvString);
	
	//레벨이 변경되었다고 알림. 레벨변경 시 동작해야 하는 곳에서 받기(EnemySpawnerTest 등)
	OnLevelChanged.Broadcast();

	GetWorld()->GetTimerManager().SetTimer(HalfTimerInGameInstance, this, &UPuckGameInstance::HalfTimer, 1.0f, true, 1.0f);
	

	//GetWorld()->GetTimerManager().SetTimer(LevelHandle, this, &UPuckGameInstance::LevelCheck, 8, false);
}

//EnemyBase::Die
void UPuckGameInstance::DoKillCount()
{
	if (KillCount >= Goal-1)
	{
		//NextLevel
		LevelNum++;
		bIsHalf = true;
		LevelCheck();
		KillCount = 0;
		return;
	}
	KillCount++;
	TotalKillCount++;
	//UE_LOG(LogTemp, Warning, TEXT("PuckGameInstance::DoKillCount %d"), KillCount);
}

void UPuckGameInstance::HalfTimer()
{
	HalfTimeSecondsGameInstance--;
	if (HalfTimeSecondsGameInstance < 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(HalfTimerInGameInstance);
		HalfTimeSecondsGameInstance = HalfTimeOrigin;
	}
}

void UPuckGameInstance::GameOver()
{
	//LevelUI
	OnGameOver.Broadcast();

	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void UPuckGameInstance::ShowGameOverWidget()
{
	if (!GameOverUIClass)
	{
		return;
	}
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		PC->bShowMouseCursor = true;
	}

	CurrentWidget = CreateWidget<UGameOverUI>(GetWorld(), GameOverUIClass);
	if (CurrentWidget)
	{
		CurrentWidget->AddToViewport();
	}

}

void UPuckGameInstance::ShowGameClearWidget()
{
	if (!GameClearUIClass)
	{
		return;
	}
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		PC->bShowMouseCursor = true;
	}
	CurrentWidget = CreateWidget<UGameClearUI>(GetWorld(), GameClearUIClass);
	if (CurrentWidget)
	{
		CurrentWidget->AddToViewport();
	}
}

void UPuckGameInstance::HideGameOverWidget()
{
	if (CurrentWidget)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
	}
}
