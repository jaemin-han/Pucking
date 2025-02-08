// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Common/CommonEnum.h"
#include "MainHUD.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()

// #pragma region CurrentMagazine
//
// 	// Image_CurrentMagazine
// 	UPROPERTY(meta = (BindWidget))
// 	class UImage* Image_CurrentMagazine;
//
// #pragma endregion

// #pragma region WeaponSlot
//
// 	// Image_Weapon
// 	UPROPERTY(meta = (BindWidget))
// 	class UImage* Image_Weapon;
//
// 	// Border_Ammo
// 	UPROPERTY(meta = (BindWidget))
// 	class UBorder* Border_Ammo;
// 	
// 	// Image_Ammo
// 	UPROPERTY(meta = (BindWidget))
// 	class UImage* Image_Ammo;
//
// 	// Text_AmmoAmount
// 	UPROPERTY(meta = (BindWidget))
// 	class UTextBlock* Text_AmmoAmount;
//
// 	// Border_Ammo_1
// 	UPROPERTY(meta = (BindWidget))
// 	class UBorder* Border_Ammo_1;
//
// 	// Image_Ammo_1
// 	UPROPERTY(meta = (BindWidget))
// 	class UImage* Image_Ammo_1;
//
// 	// Text_AmmoAmount_1
// 	UPROPERTY(meta = (BindWidget))
// 	class UTextBlock* Text_AmmoAmount_1;
//
// 	// Border_Ammo_2
// 	UPROPERTY(meta = (BindWidget))
// 	class UBorder* Border_Ammo_2;
//
// 	// Image_Ammo_2
// 	UPROPERTY(meta = (BindWidget))
// 	class UImage* Image_Ammo_2;
//
// 	// Text_AmmoAmount_2
// 	UPROPERTY(meta = (BindWidget))
// 	class UTextBlock* Text_AmmoAmount_2;
// 	
// #pragma endregion

	// CurAmmoSlot
	UPROPERTY(meta = (BindWidget))
	class UItemSlot* CurAmmoSlot;

	// CurWeaponSlot
	UPROPERTY(meta = (BindWidget))
	class UWeaponSlot* CurWeaponSlot;

protected:
	virtual void NativeOnInitialized() override;

public:
	// todo: WeaponSlot 을 받아서 해당 이미지와 AmmoAmount 를 HUD 에 반영한다
	void ApplyWeaponSlotToHUD(class UWeaponSlot* WeaponSlot);
	// todo: EquipComponent 의 HandleAmmoIndex 에서 호출되어 AmmoIndex 를 HUD 에 반영한다
	void SetCurrentMagaineImage(class UTexture2D* ItemThumbnail, EDamageType DamageType);
	// Image_Ammo 의 Tint 를 변경한다 (1, 0, 0)
	void SetAmmoImageTintRed(int32 AmmoIndex);

#pragma region MagazineUI
public:
	UPROPERTY(meta = (BindWidget))
	class USubMagazineUI* SubMagazineUI;

public:
	// 이벤트 초기화
	void BindMagazineUIEvent(class IDelegateInterface* HasMagazineInfoComponent);

	// 무기 변경 시, 총알 개수 설정
	UFUNCTION()
	void SetSubHUDMagazine(int32 MaxMagazine, int32 CurMagazine);
	
#pragma endregion

#pragma region CoolTimeUI
public:
	UPROPERTY(meta = (BindWidget))
	class USubCoolTimeUI* CoolTimeUI;
	
/*public:
	void SetJetpackUI(float Percent);
	void SetHookUI(float Percent);*/
#pragma endregion 
};
