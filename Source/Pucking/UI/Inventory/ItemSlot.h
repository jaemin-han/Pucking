// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/TagInterface.h"
#include "Item/ItemInstanceData.h"
#include "ItemSlot.generated.h"

// OnDrop 이벤트를 처리할 델리게이트, 입력으로 ItemName 을 받음
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnDropItem, FName, ItemName);

// Button_Item 이 실행될 때 호출할 델리게이트
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnItemSlotClicked, FName, ItemName);

// Equip 에 OnDrop 이 실행될 때 호출할 델리게이트
DECLARE_DYNAMIC_DELEGATE(FOnEquipDropItem);

/**
 * 
 */
UCLASS()
class PUCKING_API UItemSlot : public UUserWidget, public ITagInterface
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

	// Border_AmmoAmount
	UPROPERTY(meta = (BindWidget))
	class UBorder* Border_AmmoAmount;

	// Text_AmmoAmount
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_AmmoAmount;

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativePreConstruct() override;

	virtual FReply
	NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	                                  UDragDropOperation*& OutOperation) override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	                          UDragDropOperation* InOperation) override;

public:
#pragma region ITagInterface
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tag")
	TArray<FName> Tags;

	virtual void AddTag(const FName& Tag) override
	{
		Tags.AddUnique(Tag);
	}

	virtual void RemoveTag(const FName& Tag) override
	{
		Tags.Remove(Tag);
	}

	virtual bool HasTag(const FName& Tag) const override
	{
		return Tags.Contains(Tag);
	}
#pragma endregion

	// todo: 아마 곧 삭제될 예정, tag 로 대체
	// 해당 ItemSlot 이 어디에 포함되는지, InventoryGrid 인지 EquipWidget 인지 알고 있어라!
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemSlot", meta = (AllowPrivateAccess = true))
	FName ParentName;

	// ItemName
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemSlot", meta = (AllowPrivateAccess = true))
	FName ItemName;

	// FItemInstanceData
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemSlot", meta = (AllowPrivateAccess = true))
	FItemInstanceData ItemInstanceData;

	// FPickableItemData
	TSharedPtr<FPickableData> PickableData;
	
	// Image_InventorySlot's image
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSlot", meta = (AllowPrivateAccess = true))
	class UTexture2D* ItemThumbnail;

	// basic texture for clear
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemSlot", meta = (AllowPrivateAccess = "true"))
	class UTexture2D* BasicTexture;

	// delegate
	FOnItemSlotClicked OnItemSlotClicked;
	FOnDropItem OnDropItem;
	FOnEquipDropItem OnEquipDropItem;

public:
	void SetItemData(const FItemInstanceData& ItemData, TSharedPtr<FPickableData> InPickableData);
	void SetItemImage(class UTexture2D* Texture2D);
	void SetAmmoAmount(const int32 AmmoAmount);
	void ClearItemSlot();

	FAmmoData* GetAmmoData();

	// todo: 지금 사용중이지 않음
	static void TransferSlot(UItemSlot* SourceSlot, UItemSlot* TargetSlot);
	static void SwapSlot(UItemSlot* SlotA, UItemSlot* SlotB);

private:
	UFUNCTION()
	void OnButtonClicked();

private:
	// drag and drop operation class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemSlot", meta = (AllowPrivateAccess = true))
	TSubclassOf<class UDragDropOperation> DragDropOperationClass;
};
