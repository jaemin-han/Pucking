// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Status/LevelUI.h"
#include "World/PuckGameInstance.h"
#include "Kismet/GameplayStatics.h"

void ULevelUI::NativeConstruct()
{
	PuckGameInstance = Cast<UPuckGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
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
