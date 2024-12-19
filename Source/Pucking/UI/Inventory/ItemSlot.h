// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlot.generated.h"

struct FItemInstanceData;
/**
 * 
 */
UCLASS()
class PUCKING_API UItemSlot : public UUserWidget
{
	GENERATED_BODY()

	// Image_InventorySlot
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_InventorySlot;

	// Border_ItemAmount
	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_ItemAmount;
	
	// Text_ItemAmount
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_ItemAmount;

public:
	// ItemName
	FName ItemName;

	void SetItemData(const FItemInstanceData& ItemData);
};
