// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

#pragma region CurrentMagazine

	// Image_CurrentMagazine
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_CurrentMagazine;

#pragma endregion

#pragma region WeaponSlot

	// Image_Weapon
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Weapon;

	// Border_Ammo
	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_Ammo;
	
	// Image_Ammo
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Ammo;

	// Text_AmmoAmount
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_AmmoAmount;

	// Border_Ammo_1
	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_Ammo_1;

	// Image_Ammo_1
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Ammo_1;

	// Text_AmmoAmount_1
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_AmmoAmount_1;

	// Border_Ammo_2
	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_Ammo_2;

	// Image_Ammo_2
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_Ammo_2;

	// Text_AmmoAmount_2
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_AmmoAmount_2;
	
#pragma endregion

protected:
	virtual void NativeOnInitialized() override;

public:
	// todo: WeaponSlot 을 받아서 해당 이미지와 AmmoAmount 를 HUD 에 반영한다
	void ApplyWeaponSlotToHUD(class UWeaponSlot* WeaponSlot);
	// todo: EquipComponent 의 HandleAmmoIndex 에서 호출되어 AmmoIndex 를 HUD 에 반영한다
	void SetCurrentMagaineImage(class UTexture2D* ItemThumbnail);
	// Image_Ammo 의 Tint 를 변경한다 (1, 0, 0)
	void SetAmmoImageTintRed(int32 AmmoIndex);
};
