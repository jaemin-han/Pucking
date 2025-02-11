// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Status/GameOverUI.h"
#include "World/PuckGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void UGameOverUI::NativeConstruct()
{
	PuckGameInstance = Cast<UPuckGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (Restart_Btn)
	{
		Restart_Btn->OnClicked.AddDynamic(this, &UGameOverUI::OnRestartButtonClicked);
	}
	if (MainMenu_Btn)
	{
		MainMenu_Btn->OnClicked.AddDynamic(this, &UGameOverUI::OnMainMenuButtonClicked);
	}
}

void UGameOverUI::OnRestartButtonClicked()
{
	FName CurrenLevelName = *GetWorld()->GetMapName();
	PuckGameInstance->KillCount = 0;
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lv_GameMap"));
}

void UGameOverUI::OnMainMenuButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lv_MainMenu"));
}
