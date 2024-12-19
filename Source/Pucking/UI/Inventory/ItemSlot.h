// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemSlot.generated.h"

// Button_Item 이 실행될 때 호출할 델리게이트
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnItemSlotClicked, FName, ItemName);

struct FItemInstanceData;
/**
 * 
 */
UCLASS()
class PUCKING_API UItemSlot : public UUserWidget
{
	GENERATED_BODY()

	// Button_Item
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Item;

	// Image_InventorySlot
	UPROPERTY(meta = (BindWidget))
	class UImage* Image_InventorySlot;

	// Border_ItemAmount
	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_ItemAmount;

	// Text_ItemAmount
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_ItemAmount;

	// Image_InventorySlot's image
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSlot", meta = (AllowPrivateAccess = true))
	class UTexture2D* ItemThumbnail;

protected:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

public:
	// ItemName
	FName ItemName;

	void SetItemData(const FItemInstanceData& ItemData);

	FOnItemSlotClicked OnItemSlotClicked;

private:
	UFUNCTION()
	void OnButtonClicked();

	// draged image class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSlot", meta = (AllowPrivateAccess = true))
	TSubclassOf<class UDraggedImage> DraggedImageClass;

	// drag and drop operation class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSlot", meta = (AllowPrivateAccess = true))
	TSubclassOf<class UDragDropOperation> DragDropOperationClass;
};
