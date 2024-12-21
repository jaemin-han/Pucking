// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WeaponSlot.generated.h"

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

public:
	// ItemSlot 의 FOnDropItem delegate 에 binding 되는 함수.. 였는데 안씀
	// ItemSlots 에서 ItemName 에 해당하는 ItemSlot 을 찾아서 내부 정보만 제거
	// todo: 현재 사용처 없음
	UFUNCTION()
	void DropItem(FName ItemName);
};
