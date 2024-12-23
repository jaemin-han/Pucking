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

protected:
	virtual void NativeConstruct() override;

public:
	// UItemSlot class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UItemSlot> ItemSlotClass;

	// ItemSlot Array
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TArray<class UItemSlot*> ItemSlots;

	// 해당 WeaponSLot 의 EWeaponType
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	EWeaponType WeaponType;

	// delegate
	FOnAddItemSlot OnAddItemSlot;

	// bool
	bool bIsItemSlotCreated = false;
};
