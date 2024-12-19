// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryGrid.h"

#include "ItemSlot.h"
#include "Components/WrapBox.h"

int32 UInventoryGrid::GetSlotCount()
{
	if (WrapBox_Inventory)
	{
		return WrapBox_Inventory->GetChildrenCount();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WrapBox_Inventory is nullptr"));
		return 0;
	}
}

void UInventoryGrid::AddItemSlot(class UItemSlot* ItemSlot)
{
	if (WrapBox_Inventory)
	{
		WrapBox_Inventory->AddChild(ItemSlot);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WrapBox_Inventory is nullptr"));
	}
}
