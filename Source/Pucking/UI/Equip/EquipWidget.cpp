// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipWidget.h"

#include "WeaponSlot.h"

void UEquipWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WeaponSlot_0->WeaponType = EWeaponType::Rifle;
	WeaponSlot_1->WeaponType = EWeaponType::Shotgun;
	WeaponSlot_2->WeaponType = EWeaponType::BFG;
	WeaponSlot_3->WeaponType = EWeaponType::Hammer;

	WeaponSlot_0->AddTagToItemSlot();
	WeaponSlot_1->AddTagToItemSlot();
	WeaponSlot_2->AddTagToItemSlot();
	WeaponSlot_3->AddTagToItemSlot();
}
