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

	if (StartSlot == EndSlot)
	{
		return false;
	}
	else if (StartSlot->ParentName == "Inventory" && EndSlot->ParentName == "Equip")
	{
		StartSlot->TransferSlot(EndSlot);
		StartSlot->OnDropItem.ExecuteIfBound(StartSlot->ItemName);
	}
	else if (StartSlot->ParentName == "Equip" && EndSlot->ParentName == "Inventory")
	{
		StartSlot->SwapSlot(EndSlot);
	}
	else if (StartSlot->ParentName == "Inventory" && EndSlot->ParentName == "Inventory")
	{
		StartSlot->SwapSlot(EndSlot);
	}
	else if (StartSlot->ParentName == "Equip" && EndSlot->ParentName == "Equip")
	{
		StartSlot->SwapSlot(EndSlot);
	}

	UE_LOG(LogTemp, Warning, TEXT("%s: UItemSlot::NativeOnDrop"), *ItemName.ToString());

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

void UItemSlot::TransferSlot(UItemSlot* TargetSlot)
{
	// TargetSlot 의 ItemName 을 현재 ItemName 으로 설정
	TargetSlot->ItemName = ItemName;
	TargetSlot->ItemInstanceData = ItemInstanceData;
	TargetSlot->Image_InventorySlot->SetBrushFromTexture(ItemThumbnail);
	TargetSlot->ItemThumbnail = ItemThumbnail;
}

void UItemSlot::SwapSlot(UItemSlot* TargetSlot)
{
	UE_LOG(LogTemp, Warning, TEXT("UItemSlot::SwapSlot"));

	// this 의 ItemName 과 TargetSlot 의 ItemName 을 출력
	UE_LOG(LogTemp, Warning, TEXT("this: %s, TargetSlot: %s"), *ItemName.ToString(), *TargetSlot->ItemName.ToString());

	// 현재 ItemSlot 의 ItemName 과 TargetSlot 의 ItemName 을 Swap
	FName TempItemName = ItemName;
	ItemName = TargetSlot->ItemName;
	TargetSlot->ItemName = TempItemName;
	
	// 현재 ItemSlot 의 ItemThumbnail 과 TargetSlot 의 ItemThumbnail 을 Swap
	UTexture2D* TempItemThumbnail = this->ItemThumbnail;
	this->ItemThumbnail = TargetSlot->ItemThumbnail;
	TargetSlot->ItemThumbnail = TempItemThumbnail;
	Image_InventorySlot->SetBrushFromTexture(ItemThumbnail);
	TargetSlot->Image_InventorySlot->SetBrushFromTexture(TargetSlot->ItemThumbnail);
	
	// 현재 ItemSlot 의 ItemInstanceData 와 TargetSlot 의 ItemInstanceData 를 Swap
	FItemInstanceData TempItemInstanceData = ItemInstanceData;
	ItemInstanceData = TargetSlot->ItemInstanceData;
	TargetSlot->ItemInstanceData = TempItemInstanceData;
}

void UItemSlot::OnButtonClicked()
{
	// todo:
	// OnItemSlotClicked.ExecuteIfBound(ItemName);
	

	// debug parrent name
	UE_LOG(LogTemp, Warning, TEXT("ParentName: %s"), *ParentName.ToString());
	// debug item name
	UE_LOG(LogTemp, Warning, TEXT("ItemName: %s"), *ItemName.ToString());
}
