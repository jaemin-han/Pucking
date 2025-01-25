// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SubUI/SubMagazineUI.h"

#include "Components/TextBlock.h"


void USubMagazineUI::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void USubMagazineUI::ChangeCurMagazine(int32 CurrentM)
{
	CurMagazine->SetText(FText::AsNumber(CurrentM));
}

void USubMagazineUI::ChangeMaxMagazine(int32 MaxM)
{
	MaxMagazine->SetText(FText::AsNumber(MaxM));
}

