// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Item/ItemInstanceData.h"
#include "ItemSlot.generated.h"

// OnDrop 이벤트를 처리할 델리게이트, 입력으로 ItemName 을 받음
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnDropItem, FName, ItemName);

// Button_Item 이 실행될 때 호출할 델리게이트
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnItemSlotClicked, FName, ItemName);

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

	// basic texture for clear
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot", meta = (AllowPrivateAccess = "true"))
	class UTexture2D* BasicTexture;


protected:
	virtual void NativeConstruct() override;

	virtual FReply
	NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	                                  UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	                          UDragDropOperation* InOperation) override;

public:
	// 해당 ItemSlot 이 어디에 포함되는지, InventoryGrid 인지 EquipWidget 인지 알고 있어라!
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemSlot", meta = (AllowPrivateAccess = true))
	FName ParentName;
	
	// ItemName
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemSlot", meta = (AllowPrivateAccess = true))
	FName ItemName;

	// FItemInstanceData
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemSlot", meta = (AllowPrivateAccess = true))
	FItemInstanceData ItemInstanceData;


	// delegate
	FOnItemSlotClicked OnItemSlotClicked;
	FOnDropItem OnDropItem;

public:
	void SetItemData(const FItemInstanceData& ItemData);
	void SetItemImage(class UTexture2D* Texture2D);
	void ClearItemSlot();
	
	static void TransferSlot(UItemSlot* SourceSlot, UItemSlot* TargetSlot);
	static void SwapSlot(UItemSlot* SlotA, UItemSlot* SlotB);

private:
	UFUNCTION()
	void OnButtonClicked();

private:
	// draged image class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSlot", meta = (AllowPrivateAccess = true))
	TSubclassOf<class UDraggedImage> DraggedImageClass;

	// drag and drop operation class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSlot", meta = (AllowPrivateAccess = true))
	TSubclassOf<class UDragDropOperation> DragDropOperationClass;
};
