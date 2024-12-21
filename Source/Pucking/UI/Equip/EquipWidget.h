// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EquipWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API UEquipWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// WeaponSlot_0
	UPROPERTY(meta = (BindWidget))
	class UWeaponSlot* WeaponSlot_0;

	// WeaponSlot_1
	UPROPERTY(meta = (BindWidget))
	class UWeaponSlot* WeaponSlot_1;

	// WeaponSlot_2
	UPROPERTY(meta = (BindWidget))
	class UWeaponSlot* WeaponSlot_2;
};
