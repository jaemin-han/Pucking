// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlot.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/ItemInstanceData.h"


void UItemSlot::SetItemData(const FItemInstanceData& ItemData)
{
	// ItemName 을 ItemData 의 ItemName 으로 설정
	ItemName = FName(ItemData.ItemName.ToString());
	
	// ItemData 의 ItemThumbnail 을 Image_InventorySlot 의 Brush 로 설정
	if (Image_InventorySlot)
	{
		Image_InventorySlot->SetBrushFromTexture(ItemData.ItemThumbnail);
	}

	// ItemData 의 bStackable 이 true 이면
	if (ItemData.bStackable)
	{
		// Text_ItemAmount 의 Visibility 를 Visible 로 설정
		// Text_ItemAmount 의 Text 를 ItemData 의 StackCount 로 설정
		if (Text_ItemAmount)
		{
			Border_ItemAmount->SetVisibility(ESlateVisibility::Visible);
			Text_ItemAmount->SetText(FText::FromString(FString::FromInt(ItemData.MaxStackCount)));
		}
	}
	else
	{
		// Text_ItemAmount 의 Visibility 를 Hidden 로 설정
		if (Text_ItemAmount)
		{
			Border_ItemAmount->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}
