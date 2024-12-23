// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSlot.h"

#include "Components/HorizontalBox.h"
#include "UI/Inventory/ItemSlot.h"

void UWeaponSlot::NativeConstruct()
{
	Super::NativeConstruct();

	if (!bIsItemSlotCreated)
	{
		// create three ItemSlots
		for (int i = 0; i < 3; ++i)
		{
			auto* ItemSlot = CreateWidget<UItemSlot>(GetWorld(), ItemSlotClass);
			ItemSlot->ParentName = FName("Equip");
			// add to HorizontalBox_ItemSlot
			HorizontalBox_ItemSlot->AddChild(ItemSlot);
			OnAddItemSlot.Broadcast(WeaponType, ItemSlot);
		}
		bIsItemSlotCreated = true;
	}

	// weapon slot debug
	FString WeaponTypeString = UEnum::GetValueAsString(WeaponType);
	UE_LOG(LogTemp, Warning, TEXT("WeaponSlot %s created"), *WeaponTypeString);
}
