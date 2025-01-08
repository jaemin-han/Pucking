// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponSlot.h"

#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "UI/Inventory/ItemSlot.h"

void UWeaponSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ItemSlot_0->ParentName = FName("Equip");
	ItemSlot_1->ParentName = FName("Equip");
	ItemSlot_2->ParentName = FName("Equip");

	// add tag
	ItemSlot_0->AddTag(FName("WeaponSlot"));
	ItemSlot_1->AddTag(FName("WeaponSlot"));
	ItemSlot_2->AddTag(FName("WeaponSlot"));

	// 각 ItemSlot 에 WeaponType 을 FName 형태로 Tag 추가
	const FName WeaponTypeName = UEnum::GetValueAsName(WeaponType);
	ItemSlot_0->AddTag(WeaponTypeName);
	ItemSlot_1->AddTag(WeaponTypeName);
	ItemSlot_2->AddTag(WeaponTypeName);

	CheckAndBroadcast();

	// Image_Weapon 에 WeaponTexture 를 설정
	if (Image_Weapon)
	{
		Image_Weapon->SetBrushFromTexture(WeaponTexture);
	}
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
