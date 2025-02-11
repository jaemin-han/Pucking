// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Status/LevelUI.h"
#include "World/PuckGameInstance.h"
#include "Kismet/GameplayStatics.h"

void ULevelUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PuckGameInstance = Cast<UPuckGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	PuckGameInstance->OnGameOver.AddDynamic(this, &ULevelUI::GameOverHideWidget);
}

void ULevelUI::VisibleHalfTimer(bool bHalf)
{
	if (bHalf == false)
	{
		SetVisibility(ESlateVisibility::Hidden);
	}
	else if (bHalf == true)
	{
		SetVisibility(ESlateVisibility::Visible);
	}
}

void ULevelUI::GameOverHideWidget()
{
	SetVisibility(ESlateVisibility::Collapsed);
}