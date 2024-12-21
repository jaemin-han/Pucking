// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSlot.h"

#include "Components/HorizontalBox.h"
#include "UI/Inventory/ItemSlot.h"

void UWeaponSlot::NativeConstruct()
{
	Super::NativeConstruct();

	// 이미 생성된 ItemSlot 이 있다면 아래 코드를 실행하지 않음
	if (ItemSlots.Num() > 0)
	{
		return;
	}
	
	// create three ItemSlots
	for (int i = 0; i < 3; ++i)
	{
		auto* ItemSlot = CreateWidget<UItemSlot>(GetWorld(), ItemSlotClass);
		ItemSlot->ParentName = FName("Equip");
		// add to HorizontalBox_ItemSlot
		HorizontalBox_ItemSlot->AddChild(ItemSlot);
		ItemSlots.Add(ItemSlot);
	}
}

void UWeaponSlot::DropItem(FName ItemName)
{
	// ItemSlots 에서 ItemName 에 해당하는 ItemSlot 을 찾아서 제거
	for (int i = 0; i < ItemSlots.Num(); ++i)
	{
		if (ItemSlots[i]->ItemName == ItemName)
		{
			ItemSlots[i]->ClearItemSlot();				
			break;
		}
	}
}
