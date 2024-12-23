// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlot.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/ItemInstanceData.h"
#include "DraggedImage.h"
#include "ItemDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/Equip/EquipWidget.h"
#include "UI/Equip/WeaponSlot.h"

void UItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	UE_LOG(LogTemp, Warning, TEXT("UItemSlot::NativeConstruct"));

	// bind Button_Item
	if (Button_Item && Button_Item->OnClicked.IsBound() == false)
	{
		Button_Item->OnClicked.AddDynamic(this, &UItemSlot::OnButtonClicked);
	}
}

FReply UItemSlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// ItemName 이 비어있으면 NativeOnPreviewMouseButtonDown 을 실행하지 않음
	if (ItemName.IsNone())
	{
		return FReply::Unhandled();
	}
	
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton).NativeReply;
	}

	return Super::NativeOnPreviewMouseButtonDown(InGeometry, InMouseEvent);
}

void UItemSlot::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
                                     UDragDropOperation*& OutOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("UItemSlot::NativeOnDragDetected"));

	// auto* DraggedImage = CreateWidget<UDraggedImage>(GetWorld(), DraggedImageClass);
	// DraggedImage->Image_Dragged->SetBrushFromTexture(ItemThumbnail);

	auto* ItemDragDropOperation = Cast<UItemDragDropOperation>(
		UWidgetBlueprintLibrary::CreateDragDropOperation(DragDropOperationClass));
	// ItemDragDropOperation->DefaultDragVisual = DraggedImage;
	ItemDragDropOperation->DefaultDragVisual = this;
	ItemDragDropOperation->ItemThumbnail = ItemThumbnail;
	ItemDragDropOperation->ItemSlot = this;

	OutOperation = ItemDragDropOperation;

	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                             UDragDropOperation* InOperation)
{
	auto* ItemDragDropOperation = Cast<UItemDragDropOperation>(InOperation);

	auto* StartSlot = ItemDragDropOperation->ItemSlot;
	auto* EndSlot = this;
	
	UE_LOG(LogTemp, Warning, TEXT("%s: UItemSlot::NativeOnDrop"), *ItemName.ToString());

	// todo: equip slot의 아이템 -> equip slot의 비어있는 slot 할 때 같게 취급되는 문제 해결
	if (StartSlot == EndSlot)
	{
		return false;
	}
	else if (StartSlot->ParentName == "Inventory" && EndSlot->ParentName == "Equip")
	{
		TransferSlot(StartSlot, EndSlot);
		StartSlot->OnDropItem.ExecuteIfBound(StartSlot->ItemName);
	}
	else if (StartSlot->ParentName == "Equip" && EndSlot->ParentName == "Inventory")
	{
		SwapSlot(StartSlot, EndSlot);
	}
	else if (StartSlot->ParentName == "Inventory" && EndSlot->ParentName == "Inventory")
	{
		SwapSlot(StartSlot, EndSlot);
	}
	else if (StartSlot->ParentName == "Equip" && EndSlot->ParentName == "Equip")
	{
		SwapSlot(StartSlot, EndSlot);
	}


	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}


void UItemSlot::SetItemData(const FItemInstanceData& ItemData)
{
	// ItemName 을 ItemData 의 ItemName 으로 설정
	ItemName = FName(ItemData.ItemName.ToString());
	ItemInstanceData = ItemData;
	ParentName = FName("Inventory");

	// ItemData 의 ItemThumbnail 을 Image_InventorySlot 의 Brush 로 설정
	if (Image_InventorySlot)
	{
		Image_InventorySlot->SetBrushFromTexture(ItemData.ItemThumbnail);
		ItemThumbnail = ItemData.ItemThumbnail;
	}

	// ItemData 의 bStackable 이 true 이면
	if (ItemData.bStackable)
	{
		// Text_ItemAmount 의 Visibility 를 Visible 로 설정
		// Text_ItemAmount 의 Text 를 ItemData 의 StackCount 로 설정
		if (Text_ItemAmount)
		{
			Border_ItemAmount->SetVisibility(ESlateVisibility::Visible);
			Text_ItemAmount->SetText(FText::FromString(FString::FromInt(ItemData.MaxStackCount)));
		}
	}
	else
	{
		// Text_ItemAmount 의 Visibility 를 Hidden 로 설정
		if (Text_ItemAmount)
		{
			Border_ItemAmount->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void UItemSlot::SetItemImage(class UTexture2D* Texture2D)
{
	if (Image_InventorySlot)
	{
		Image_InventorySlot->SetBrushFromTexture(Texture2D);
	}
}

void UItemSlot::ClearItemSlot()
{
	// ParentName 을 제외한 ItemSlot 의 ItemName 을 초기화
	ItemName = FName();
	ItemInstanceData = FItemInstanceData();
	Image_InventorySlot->SetBrushFromTexture(BasicTexture);
}

void UItemSlot::TransferSlot(UItemSlot* SourceSlot, UItemSlot* TargetSlot)
{
	if (!SourceSlot || !TargetSlot)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Slot(s) provided for TransferSlot"));
		return;
	}

	// SourceSlot의 ItemName, ItemInstanceData, ItemThumbnail을 TargetSlot에 설정
	TargetSlot->ItemName = SourceSlot->ItemName;
	TargetSlot->ItemInstanceData = SourceSlot->ItemInstanceData;
	TargetSlot->ItemThumbnail = SourceSlot->ItemThumbnail;

	// TargetSlot의 이미지 업데이트
	TargetSlot->SetItemImage(SourceSlot->ItemThumbnail);
}

void UItemSlot::SwapSlot(UItemSlot* SlotA, UItemSlot* SlotB)
{
	if (!SlotA || !SlotB)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Slot(s) provided for SwapSlot"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("UItemSlot::SwapSlot"));

	// SlotA와 SlotB의 ItemName 출력
	UE_LOG(LogTemp, Warning, TEXT("SlotA: %s, SlotB: %s"), *SlotA->ItemName.ToString(), *SlotB->ItemName.ToString());

	// ItemName Swap
	Swap(SlotA->ItemName, SlotB->ItemName);

	// ItemThumbnail Swap
	Swap(SlotA->ItemThumbnail, SlotB->ItemThumbnail);
	SlotA->SetItemImage(SlotA->ItemThumbnail);
	SlotB->SetItemImage(SlotB->ItemThumbnail);

	// ItemInstanceData Swap
	Swap(SlotA->ItemInstanceData, SlotB->ItemInstanceData);
}

void UItemSlot::OnButtonClicked()
{
	// todo:
	// OnItemSlotClicked.ExecuteIfBound(ItemName);
	

	// debug parrent name
	UE_LOG(LogTemp, Warning, TEXT("ParentName: %s"), *ParentName.ToString());
	// debug item name
	UE_LOG(LogTemp, Warning, TEXT("ItemName: %s"), *ItemName.ToString());
	// debug ItemInstanceData.ItemOptionDescription
	UE_LOG(LogTemp, Warning, TEXT("ItemOptionDescription: %s"), *ItemInstanceData.ItemOptionDescription);
}
