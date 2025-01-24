// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Status/MainMenuUI.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (GameStart_Btn)
	{
		GameStart_Btn->OnClicked.AddDynamic(this, &UMainMenuUI::OnStartButtonClicked);
	}
}

void UMainMenuUI::OnStartButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lv_GameMap"));
}
