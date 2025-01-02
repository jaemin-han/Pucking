// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "UI/Equip/WeaponSlot.h"
#include "UI/Inventory/ItemSlot.h"

void UMainHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	// 모든 text 를 "0" 으로 초기화
	Text_AmmoAmount->SetText(FText::FromString("0"));
	Text_AmmoAmount_1->SetText(FText::FromString("0"));
	Text_AmmoAmount_2->SetText(FText::FromString("0"));
}

void UMainHUD::ApplyWeaponSlotToHUD(class UWeaponSlot* WeaponSlot)
{
	// WeaponSlot 의 WeaponTexture 를 Image_Weapon 에 설정
	Image_Weapon->SetBrushFromTexture(WeaponSlot->WeaponTexture);

	// WeaponSlot 의 ItemSlot_0, ItemSlot_1, ItemSlot_2 의 ItemThumbnail, ItemInstanceData->AmmoData.AmmoCount 를 HUD 에 반영
	Image_Ammo->SetBrushFromTexture(WeaponSlot->ItemSlot_0->ItemThumbnail);
	Text_AmmoAmount->SetText(
		FText::FromString(FString::FromInt(WeaponSlot->ItemSlot_0->ItemInstanceData.AmmoData.AmmoCount)));

	Image_Ammo_1->SetBrushFromTexture(WeaponSlot->ItemSlot_1->ItemThumbnail);
	Text_AmmoAmount_1->SetText(
		FText::FromString(FString::FromInt(WeaponSlot->ItemSlot_1->ItemInstanceData.AmmoData.AmmoCount)));

	Image_Ammo_2->SetBrushFromTexture(WeaponSlot->ItemSlot_2->ItemThumbnail);
	Text_AmmoAmount_2->SetText(
		FText::FromString(FString::FromInt(WeaponSlot->ItemSlot_2->ItemInstanceData.AmmoData.AmmoCount)));
}

void UMainHUD::SetCurrentMagaineImage(class UTexture2D* ItemThumbnail)
{
	Image_CurrentMagazine->SetBrushFromTexture(ItemThumbnail);
}

void UMainHUD::SetAmmoImageTintRed(int32 AmmoIndex)
{
	if (AmmoIndex == 0)
	{
		Image_Ammo->SetBrushTintColor(FLinearColor(1, 0, 0));
		Image_Ammo_1->SetBrushTintColor(FLinearColor(1, 1, 1));
		Image_Ammo_2->SetBrushTintColor(FLinearColor(1, 1, 1));
	}
	else if (AmmoIndex == 1)
	{
		Image_Ammo->SetBrushTintColor(FLinearColor(1, 1, 1));
		Image_Ammo_1->SetBrushTintColor(FLinearColor(1, 0, 0));
		Image_Ammo_2->SetBrushTintColor(FLinearColor(1, 1, 1));
	}
	else if (AmmoIndex == 2)
	{
		Image_Ammo->SetBrushTintColor(FLinearColor(1, 1, 1));
		Image_Ammo_1->SetBrushTintColor(FLinearColor(1, 1, 1));
		Image_Ammo_2->SetBrushTintColor(FLinearColor(1, 0, 0));
	}
	else
	{
		// all white
		Image_Ammo->SetBrushTintColor(FLinearColor(1, 1, 1));
		Image_Ammo_1->SetBrushTintColor(FLinearColor(1, 1, 1));
		Image_Ammo_2->SetBrushTintColor(FLinearColor(1, 1, 1));
	}
}
