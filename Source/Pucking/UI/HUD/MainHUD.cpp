// Fill out your copyright notice in the Description page of Project Settings.


#include "MainHUD.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Interfaces/DelegateInterface.h"
#include "SubUI/SubCoolTimeUI.h"
#include "SubUI/SubMagazineUI.h"
#include "UI/Equip/WeaponSlot.h"
#include "UI/Inventory/ItemSlot.h"

void UMainHUD::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	// // 모든 text 를 "0" 으로 초기화
	// Text_AmmoAmount->SetText(FText::FromString("0"));
	// Text_AmmoAmount_1->SetText(FText::FromString("0"));
	// Text_AmmoAmount_2->SetText(FText::FromString("0"));
}

void UMainHUD::ApplyWeaponSlotToHUD(class UWeaponSlot* WeaponSlot)
{
	// // WeaponSlot 의 WeaponTexture 를 Image_Weapon 에 설정
	// Image_Weapon->SetBrushFromTexture(WeaponSlot->WeaponTexture);
	//
	//
	// // WeaponSlot 의 ItemSlot_0, ItemSlot_1, ItemSlot_2 의 ItemThumbnail, ItemInstanceData->AmmoData.AmmoCount 를 HUD 에 반영
	// Image_Ammo->SetBrushFromTexture(WeaponSlot->ItemSlot_0->ItemThumbnail);
	// Text_AmmoAmount->SetText(
	// 	FText::FromString(FString::FromInt(AmmoData0 ? AmmoData0->AmmoCount : 0)));
	//
	// Image_Ammo_1->SetBrushFromTexture(WeaponSlot->ItemSlot_1->ItemThumbnail);
	// Text_AmmoAmount_1->SetText(
	// 	FText::FromString(FString::FromInt(AmmoData1 ? AmmoData1->AmmoCount : 0)));
	//
	// Image_Ammo_2->SetBrushFromTexture(WeaponSlot->ItemSlot_2->ItemThumbnail);
	// Text_AmmoAmount_2->SetText(
	// 	FText::FromString(FString::FromInt(AmmoData2 ? AmmoData2->AmmoCount : 0)));
	CurWeaponSlot->SetWeaponImage(WeaponSlot->WeaponTexture);
	FAmmoData* AmmoData0 = WeaponSlot->ItemSlot_0->GetAmmoData();
	FAmmoData* AmmoData1 = WeaponSlot->ItemSlot_1->GetAmmoData();
	FAmmoData* AmmoData2 = WeaponSlot->ItemSlot_2->GetAmmoData();
	if (AmmoData0)
	{
		CurWeaponSlot->ItemSlot_0->SetItemImage(WeaponSlot->ItemSlot_0->ItemThumbnail);
		CurWeaponSlot->ItemSlot_0->SetAmmoAmount(AmmoData0->AmmoCount);
		CurWeaponSlot->ItemSlot_0->SetDamageTypeImage(AmmoData0->DamageType);
	}
	else
	{
		CurWeaponSlot->ItemSlot_0->ClearItemSlot();
	}
	if (AmmoData1)
	{
		CurWeaponSlot->ItemSlot_1->SetItemImage(WeaponSlot->ItemSlot_1->ItemThumbnail);
		CurWeaponSlot->ItemSlot_1->SetAmmoAmount(AmmoData1->AmmoCount);
		CurWeaponSlot->ItemSlot_1->SetDamageTypeImage(AmmoData1->DamageType);
	}
	else
	{
		CurWeaponSlot->ItemSlot_1->ClearItemSlot();
	}
	if (AmmoData2)
	{
		CurWeaponSlot->ItemSlot_2->SetItemImage(WeaponSlot->ItemSlot_2->ItemThumbnail);
		CurWeaponSlot->ItemSlot_2->SetAmmoAmount(AmmoData2->AmmoCount);
		CurWeaponSlot->ItemSlot_2->SetDamageTypeImage(AmmoData2->DamageType);
	}
	else
	{
		CurWeaponSlot->ItemSlot_2->ClearItemSlot();
	}
}

void UMainHUD::SetCurrentMagaineImage(class UTexture2D* ItemThumbnail, EDamageType DamageType)
{
	// Image_CurrentMagazine->SetBrushFromTexture(ItemThumbnail);
	CurAmmoSlot->SetItemImage(ItemThumbnail);
	CurAmmoSlot->SetDamageTypeImage(DamageType);
}

void UMainHUD::SetAmmoImageTintRed(int32 AmmoIndex)
{
	// if (AmmoIndex == 0)
	// {
	// 	Image_Ammo->SetBrushTintColor(FLinearColor(1, 0, 0));
	// 	Image_Ammo_1->SetBrushTintColor(FLinearColor(1, 1, 1));
	// 	Image_Ammo_2->SetBrushTintColor(FLinearColor(1, 1, 1));
	// }
	// else if (AmmoIndex == 1)
	// {
	// 	Image_Ammo->SetBrushTintColor(FLinearColor(1, 1, 1));
	// 	Image_Ammo_1->SetBrushTintColor(FLinearColor(1, 0, 0));
	// 	Image_Ammo_2->SetBrushTintColor(FLinearColor(1, 1, 1));
	// }
	// else if (AmmoIndex == 2)
	// {
	// 	Image_Ammo->SetBrushTintColor(FLinearColor(1, 1, 1));
	// 	Image_Ammo_1->SetBrushTintColor(FLinearColor(1, 1, 1));
	// 	Image_Ammo_2->SetBrushTintColor(FLinearColor(1, 0, 0));
	// }
	// else
	// {
	// 	// all white
	// 	Image_Ammo->SetBrushTintColor(FLinearColor(1, 1, 1));
	// 	Image_Ammo_1->SetBrushTintColor(FLinearColor(1, 1, 1));
	// 	Image_Ammo_2->SetBrushTintColor(FLinearColor(1, 1, 1));
	// }

	// 기본 색상 (R=0.520996,G=1.000000,B=0.099899,A=1.000000)
	// 빨강 색상 (R=1.000000,G=0.000000,B=0.127982,A=1.000000)
	// FLinearColor NormalColor = FLinearColor(0.520996, 1.000000, 0.099899, 1.000000);
	FLinearColor NormalColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
	FLinearColor RedColor = FLinearColor(1.000000, 0.000000, 0.127982, 1.000000);

	if (AmmoIndex == 0)
	{
		CurWeaponSlot->ItemSlot_0->SetColorAndOpacity(RedColor);
		CurWeaponSlot->ItemSlot_1->SetColorAndOpacity(NormalColor);
		CurWeaponSlot->ItemSlot_2->SetColorAndOpacity(NormalColor);
	}
	else if (AmmoIndex == 1)
	{
		CurWeaponSlot->ItemSlot_0->SetColorAndOpacity(NormalColor);
		CurWeaponSlot->ItemSlot_1->SetColorAndOpacity(RedColor);
		CurWeaponSlot->ItemSlot_2->SetColorAndOpacity(NormalColor);
	}
	else if (AmmoIndex == 2)
	{
		CurWeaponSlot->ItemSlot_0->SetColorAndOpacity(NormalColor);
		CurWeaponSlot->ItemSlot_1->SetColorAndOpacity(NormalColor);
		CurWeaponSlot->ItemSlot_2->SetColorAndOpacity(RedColor);
	}
	else
	{
		CurWeaponSlot->ItemSlot_0->SetColorAndOpacity(NormalColor);
		CurWeaponSlot->ItemSlot_1->SetColorAndOpacity(NormalColor);
		CurWeaponSlot->ItemSlot_2->SetColorAndOpacity(NormalColor);
	}
}

void UMainHUD::BindMagazineUIEvent(IDelegateInterface* HasMagazineInfoComponent)
{
	if(SubMagazineUI && HasMagazineInfoComponent)
	{
		HasMagazineInfoComponent->DelegateFireComplete(TDelegate<void(int32)>::CreateUObject(SubMagazineUI, &USubMagazineUI::ChangeCurMagazine));
		HasMagazineInfoComponent->DelegateReloadComplete(TDelegate<void(int32)>::CreateUObject(SubMagazineUI, &USubMagazineUI::ChangeMaxMagazine));
	}
}

void UMainHUD::SetSubHUDMagazine(int32 MaxMagazine, int32 CurMagazine)
{
	if(SubMagazineUI)
	{
		SubMagazineUI->ChangeMaxMagazine(MaxMagazine);
		SubMagazineUI->ChangeCurMagazine(CurMagazine);
	}
}

/*void UMainHUD::SetJetpackUI(float Percent)
{
	if(CoolTimeUI)
	{
		CoolTimeUI->SetJetpackGauge(Percent);
	}
}

void UMainHUD::SetHookUI(float Percent)
{
	if(CoolTimeUI)
	{
		CoolTimeUI->SetHookGauge(Percent);
	}
}*/
