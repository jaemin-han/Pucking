// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SubHUD.h"

#include "Components/TextBlock.h"


void USubHUD::NativeConstruct()
{
	Super::NativeConstruct();
	
}

void USubHUD::ChangeWeaponImg(EWeaponType WeaponType)
{
	
}

void USubHUD::ChangeCurMagazine(int32 CurrentM)
{
	CurMagazine->SetText(FText::AsNumber(CurrentM));
}

void USubHUD::ChangeMaxMagazine(int32 MaxM)
{
	MaxMagazine->SetText(FText::AsNumber(MaxM));
}

