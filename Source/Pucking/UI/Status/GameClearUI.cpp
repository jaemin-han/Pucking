// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Status/GameClearUI.h"
#include "World/PuckGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UGameClearUI::NativeConstruct()
{
	PuckGameInstance = Cast<UPuckGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	PuckGameInstance->OnGameClear.AddDynamic(this, &UGameClearUI::SetTotalScoreInTextBlock);
	if (Restart_Btn)
	{
		Restart_Btn->OnClicked.AddDynamic(this, &UGameClearUI::OnRestartButtonClicked);
	}
	if (Exit_Btn)
	{
		Exit_Btn->OnClicked.AddDynamic(this, &UGameClearUI::OnMainExitButtonClicked);
	}

}

void UGameClearUI::OnRestartButtonClicked()
{
	FName CurrenLevelName = *GetWorld()->GetMapName();

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lv_GameMap"));
}

void UGameClearUI::OnMainExitButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lv_MainMenu"));
}

void UGameClearUI::SetTotalScoreInTextBlock()
{
	if (TotalScoreText)
	{
		TotalScoreText->SetText(FText::AsNumber(PuckGameInstance->TotalKillCount));
	}
}
