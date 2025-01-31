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

public:
	// Btn_Close
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Close;

protected:
	virtual void NativeOnInitialized() override;

private:
	// UniformGridPanel_Inventory
	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel* UniformGridPanel_Inventory;

public:
	// max slot count
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 MaxSlotCount = 36;

	// row size
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 RowSize = 6;

	// row and column
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 RowIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inventory")
	int32 ColumnIndex = 0;

public:
	int32 GetSlotCount();
	void AddItemSlot(class UItemSlot* ItemSlot);
	// todo: 원래 Item Drop 시 사용했었는데 지금은 사용하지 않음
	class UItemSlot* FindItemSlot(FName ItemName);
};
