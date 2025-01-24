// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryGrid.h"

#include "ItemSlot.h"
#include "Components/UniformGridPanel.h"


int32 UInventoryGrid::GetSlotCount()
{
	if (UniformGridPanel_Inventory)
	{
		return UniformGridPanel_Inventory->GetChildrenCount();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WrapBox_Inventory is nullptr"));
		return 0;
	}
}

void UInventoryGrid::AddItemSlot(class UItemSlot* ItemSlot)
{
	if (UniformGridPanel_Inventory)
	{
		// WrapBox_Inventory->AddChild(ItemSlot);
		UniformGridPanel_Inventory->AddChildToUniformGrid(ItemSlot, RowIndex, ColumnIndex);
		ColumnIndex++;
		if (ColumnIndex >= RowSize)
		{
			ColumnIndex = 0;
			RowIndex++;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WrapBox_Inventory is nullptr"));
	}
}

// todo: 원래 Item Drop 시 사용했었는데 지금은 사용하지 않음
UItemSlot* UInventoryGrid::FindItemSlot(FName ItemName)
{
	if (UniformGridPanel_Inventory)
	{
		for (int32 i = 0; i < UniformGridPanel_Inventory->GetChildrenCount(); ++i)
		{
			auto* ItemSlot = Cast<UItemSlot>(UniformGridPanel_Inventory->GetChildAt(i));
			if (ItemSlot && ItemSlot->ItemName == ItemName)
			{
				return ItemSlot;
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WrapBox_Inventory is nullptr"));
	}

	return nullptr;
}
