// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InventoryGrid.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API UInventoryGrid : public UUserWidget
{
	GENERATED_BODY()

	// WrapBox_Inventory
	UPROPERTY(meta = (BindWidget))
	class UWrapBox* WrapBox_Inventory;


public:
	// max slot count
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 MaxSlotCount = 45;

public:
	int32 GetSlotCount();
	void AddItemSlot(class UItemSlot* ItemSlot);
	// todo: 원래 Item Drop 시 사용했었는데 지금은 사용하지 않음
	class UItemSlot* FindItemSlot(FName ItemName);
};
