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

void UItemSlot::NativeConstruct()
{
	Super::NativeConstruct();

	// bind Button_Item
	if (Button_Item)
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

	auto* ItemDragDropOperation = Cast<UItemDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(DragDropOperationClass));
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

	UE_LOG(LogTemp, Warning, TEXT("%s: UItemSlot::NativeOnDrop"), *ItemName.ToString());
	
	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}


void UItemSlot::SetItemData(const FItemInstanceData& ItemData)
{
	// ItemName 을 ItemData 의 ItemName 으로 설정
	ItemName = FName(ItemData.ItemName.ToString());
	
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

void UItemSlot::OnButtonClicked()
{
	// todo:
	// OnItemSlotClicked.ExecuteIfBound(ItemName);
}