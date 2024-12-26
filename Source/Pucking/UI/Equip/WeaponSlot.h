// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Common/CommonEnum.h"
#include "WeaponSlot.generated.h"

// delegate EWeaponType WeaponType, class UItemSlot* ItemSlot
// WeaponItemSlots 에 WeaponType 에 해당하는 ItemSlot 을 추가
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAddItemSlot, EWeaponType, WeaponType, class UItemSlot*, ItemSlot);


/**
 * 
 */
UCLASS()
class PUCKING_API UWeaponSlot : public UUserWidget
{
	GENERATED_BODY()

	// HorizontalBox_ItemSlot
	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox* HorizontalBox_ItemSlot;

	// ItemSlot_0
	UPROPERTY(meta = (BindWidget))
	class UItemSlot* ItemSlot_0;

	// ItemSlot_1
	UPROPERTY(meta = (BindWidget))
	class UItemSlot* ItemSlot_1;

	// ItemSlot_2
	UPROPERTY(meta = (BindWidget))
	class UItemSlot* ItemSlot_2;

protected:
	virtual void NativeOnInitialized() override;

private:
	// OnAddItemSlot 이 바인딩 되어있는지 확인하고 BroadcastOnAddItemSlot 을 실행
	void CheckAndBroadcast();

public:
	// UItemSlot class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UItemSlot> ItemSlotClass;

	// 해당 WeaponSLot 의 EWeaponType
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;

	// delegate
	FOnAddItemSlot OnAddItemSlot;

};
