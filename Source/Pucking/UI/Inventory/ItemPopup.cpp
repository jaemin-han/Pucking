// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemPopup.h"

#include "ItemSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include "Item/OptionDataAsset.h"

bool UItemPopup::UpdateItemPopup(class UItemSlot* ItemSlot)
{
	if (ItemSlot->IsEmpty())
		return false;

	// Text_ItemName
	if (Text_ItemName)
	{
		// ItemRarity 에 따라 다른 색상 적용
		switch (ItemSlot->ItemData.ItemRarity)
		{
		case EItemRarity::Magic:
			Text_ItemName->SetColorAndOpacity(MagicFontColor);
			break;
		case EItemRarity::Rare:
			Text_ItemName->SetColorAndOpacity(RareFontColor);
			break;
		default:
			break;
		}

		Text_ItemName->SetText(ItemSlot->ItemData.ItemName);
	}

	// ItemBackground
	if (Image_Background)
	{
		// ItemRarity 에 따라 다른 색상 적용
		switch (ItemSlot->ItemData.ItemRarity)
		{
		case EItemRarity::Normal:
			break;
		case EItemRarity::Magic:
			Image_Background->SetColorAndOpacity(MagicBackgroundColor);
			break;
		case EItemRarity::Rare:
			Image_Background->SetColorAndOpacity(RareBackgroundColor);
			break;
		default:
			break;
		}
	}

	// Image_Item

	if (Image_Item)
	{
		Image_Item->SetBrushFromTexture(ItemSlot->ItemThumbnail);
	}

	// UGP_MainOptions clear
	if (UGP_MainOptions)
	{
		UGP_MainOptions->ClearChildren();
	}

	// get ammo data
	FAmmoData* AmmoData = ItemSlot->GetAmmoData();
	if (AmmoData)
	{
		// 0, 0 에는 WeaponType 출력
		auto* WeaponTypeText = NewObject<UTextBlock>(UGP_MainOptions);
		// font 적용
		WeaponTypeText->SetFont(Font);
		WeaponTypeText->SetText(FText::FromString(TEXT("Weapon Type: ")));
		UGP_MainOptions->AddChildToUniformGrid(WeaponTypeText, 0, 0);

		// 0, 1 에는 WeaponType 출력
		auto* SpecificWeaponTypeText = NewObject<UTextBlock>(UGP_MainOptions);
		SpecificWeaponTypeText->SetFont(Font);
		SpecificWeaponTypeText->SetColorAndOpacity(FontColor);
		FString EnumString = UEnum::GetValueAsString(AmmoData->WeaponType);
		EnumString = EnumString.Mid(EnumString.Find(TEXT("::")) + 2); // "::" 이후의 부분만 추출
		SpecificWeaponTypeText->SetText(FText::FromString(EnumString));
		UGP_MainOptions->AddChildToUniformGrid(SpecificWeaponTypeText, 0, 1);

		// 1, 0 에는 DamageType 출력
		auto* DamageTypeText = NewObject<UTextBlock>(UGP_MainOptions);
		DamageTypeText->SetFont(Font);
		DamageTypeText->SetText(FText::FromString(TEXT("Damage Type: ")));
		UGP_MainOptions->AddChildToUniformGrid(DamageTypeText, 1, 0);

		// 1, 1 에는 DamageType 출력
		auto* SpecificDamageTypeText = NewObject<UTextBlock>(UGP_MainOptions);
		SpecificDamageTypeText->SetFont(Font);
		SpecificDamageTypeText->SetColorAndOpacity(FontColor);
		EnumString = UEnum::GetValueAsString(AmmoData->DamageType);
		EnumString = EnumString.Mid(EnumString.Find(TEXT("::")) + 2); // "::" 이후의 부분만 추출
		SpecificDamageTypeText->SetText(FText::FromString(EnumString));
		UGP_MainOptions->AddChildToUniformGrid(SpecificDamageTypeText, 1, 1);

		// 2, 0 에는 Damage 출력
		auto* DamageText = NewObject<UTextBlock>(UGP_MainOptions);
		DamageText->SetFont(Font);
		DamageText->SetText(FText::FromString(TEXT("Damage: ")));
		UGP_MainOptions->AddChildToUniformGrid(DamageText, 2, 0);

		// 2, 1 에는 Damage 출력
		auto* SpecificDamageText = NewObject<UTextBlock>(UGP_MainOptions);
		SpecificDamageText->SetFont(Font);
		SpecificDamageText->SetColorAndOpacity(FontColor);
		SpecificDamageText->SetText(FText::FromString(FString::FromInt(AmmoData->AmmoDamage)));
		UGP_MainOptions->AddChildToUniformGrid(SpecificDamageText, 2, 1);

		// 3, 0 에는 Critical Rate 출력
		auto* CriticalRateText = NewObject<UTextBlock>(UGP_MainOptions);
		CriticalRateText->SetFont(Font);
		CriticalRateText->SetText(FText::FromString(TEXT("Critical Rate: ")));
		UGP_MainOptions->AddChildToUniformGrid(CriticalRateText, 3, 0);

		// 3, 1 에는 Critical Rate 출력
		auto* SpecificCriticalRateText = NewObject<UTextBlock>(UGP_MainOptions);
		SpecificCriticalRateText->SetFont(Font);
		SpecificCriticalRateText->SetColorAndOpacity(FontColor);
		SpecificCriticalRateText->SetText(FText::FromString(FString::FromInt(AmmoData->CriticalRate) + TEXT("%")));
		UGP_MainOptions->AddChildToUniformGrid(SpecificCriticalRateText, 3, 1);

		// 4, 0 에는 Critical Multiplier 출력
		auto* CriticalMultiplierText = NewObject<UTextBlock>(UGP_MainOptions);
		CriticalMultiplierText->SetFont(Font);
		CriticalMultiplierText->SetText(FText::FromString(TEXT("Critical Multiplier: ")));
		UGP_MainOptions->AddChildToUniformGrid(CriticalMultiplierText, 4, 0);

		// 4, 1 에는 Critical Multiplier 출력
		auto* SpecificCriticalMultiplierText = NewObject<UTextBlock>(UGP_MainOptions);
		SpecificCriticalMultiplierText->SetFont(Font);
		SpecificCriticalMultiplierText->SetColorAndOpacity(FontColor);
		SpecificCriticalMultiplierText->SetText(
			FText::FromString(FString::SanitizeFloat(AmmoData->CriticalMultiplier) + TEXT("%")));
		UGP_MainOptions->AddChildToUniformGrid(SpecificCriticalMultiplierText, 4, 1);
	}

	// ItemSlot -> ItemData -> ItemOptions
	// 각 ItemOption 의 OptionDescription 을 출력
	if (UGP_AddOptions)
	{
		UGP_AddOptions->ClearChildren();
		for (int i = 0; i < ItemSlot->ItemData.OriginalItemOptions.Num(); ++i)
		{
			auto* OptionData = ItemSlot->ItemData.OriginalItemOptions[i];

			// i, 0 에는 OptionType 출력
			auto* OptionTypeText = NewObject<UTextBlock>(UGP_AddOptions);
			OptionTypeText->SetFont(Font);
			OptionTypeText->SetText(FText::FromString(OptionData->GetOptionString()));
			UGP_AddOptions->AddChildToUniformGrid(OptionTypeText, i, 0);

			// i, 1 에는 OptionValue 출력
			auto* OptionValueText = NewObject<UTextBlock>(UGP_AddOptions);
			OptionValueText->SetFont(Font);
			OptionValueText->SetColorAndOpacity(FontColor);
			FString OptionValueString = FString::SanitizeFloat(OptionData->GetOptionValue());

			// GetOptionType 에 따라 다른 출력 방식
			switch (OptionData->GetOptionType())
			{
			case EOptionType::CriticalRate:
				OptionValueString += TEXT("%");
				break;
			case EOptionType::CriticalMultiplier:
				OptionValueString += TEXT("%");
				break;
			default:
				break;
			}

			OptionValueText->SetText(FText::FromString(OptionValueString));
			UGP_AddOptions->AddChildToUniformGrid(OptionValueText, i, 1);
		}
	}

	return true;
}
