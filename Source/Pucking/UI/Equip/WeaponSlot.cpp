// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSlot.h"

#include "Components/HorizontalBox.h"
#include "UI/Inventory/ItemSlot.h"

void UWeaponSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	ItemSlot_0->ParentName = FName("Equip");
	ItemSlot_1->ParentName = FName("Equip");
	ItemSlot_2->ParentName = FName("Equip");
	
	CheckAndBroadcast();

}

void UWeaponSlot::CheckAndBroadcast()
{
	UE_LOG(LogTemp, Warning, TEXT("UWeaponSlot::CheckAndBroadcast"));
	if (OnAddItemSlot.IsBound())
	{
		OnAddItemSlot.Broadcast(WeaponType, ItemSlot_0);
		OnAddItemSlot.Broadcast(WeaponType, ItemSlot_1);
		OnAddItemSlot.Broadcast(WeaponType, ItemSlot_2);
	}
	else
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UWeaponSlot::CheckAndBroadcast, 0.1f, false);
	}
}
