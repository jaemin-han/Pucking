// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSlot.h"

#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Item/ItemInstanceData.h"
#include "ItemDragDropOperation.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "UI/Equip/EquipWidget.h"
#include "UI/Equip/WeaponSlot.h"

void UItemSlot::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UItemSlot::NativePreConstruct()
{
	Super::NativePreConstruct();
	// bind Button_Item
	if (Button_Item && Button_Item->OnClicked.IsBound() == false)
	{
		Button_Item->OnClicked.AddDynamic(this, &UItemSlot::OnButtonClicked);
	}
}

FReply UItemSlot::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: UItemSlot::NativeOnPreviewMouseButtonDown"), *GetName());

	// ItemName 이 비어있으면 NativeOnPreviewMouseButtonDown 을 실행하지 않음
	if (IsEmpty())
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
	UE_LOG(LogTemp, Warning, TEXT("%s: UItemSlot::NativeOnDragDetected"), *GetName());

	auto* ItemDragDropOperation = Cast<UItemDragDropOperation>(
		UWidgetBlueprintLibrary::CreateDragDropOperation(DragDropOperationClass));
	ItemDragDropOperation->DefaultDragVisual = this;
	ItemDragDropOperation->ItemThumbnail = ItemThumbnail;
	ItemDragDropOperation->ItemSlot = this;

	OutOperation = ItemDragDropOperation;

	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
}

bool UItemSlot::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
                             UDragDropOperation* InOperation)
{
	UE_LOG(LogTemp, Warning, TEXT("%s: UItemSlot::NativeOnDrop"), *ItemName.ToString());

	auto* ItemDragDropOperation = Cast<UItemDragDropOperation>(InOperation);
	auto* StartSlot = ItemDragDropOperation->ItemSlot;
	auto* EndSlot = this;

	auto* StartAmmoData = StartSlot->GetAmmoData();
	auto* EndAmmoData = EndSlot->GetAmmoData();

	FName StartWeaponType = StartAmmoData ? UEnum::GetValueAsName(StartAmmoData->WeaponType) : NAME_None;
	FName EndWeaponType = EndAmmoData ? UEnum::GetValueAsName(EndAmmoData->WeaponType) : NAME_None;

	FName StartDamageType = StartAmmoData ? UEnum::GetValueAsName(StartAmmoData->DamageType) : NAME_None;
	FName EndDamageType = EndAmmoData ? UEnum::GetValueAsName(EndAmmoData->DamageType) : NAME_None;

	// todo: equip slot의 아이템 -> equip slot의 비어있는 slot 할 때 같게 취급되는 문제 해결
	// 긴급한 문제는 아닌걸로 보이니, 이후에 drag&drop 기능을 직접 구현해서 고치든가.. 해야함
	if (StartSlot == EndSlot)
	{
		return false;
	}

	// StartSlot 이 WeaponSlot 이고,
	if (StartSlot->HasTag("WeaponSlot"))
	{
		// EndSlot 은 비어있거나
		if (EndSlot->HasTag("InventorySlot") && EndSlot->IsEmpty())
		{
			SwapSlot(StartSlot, EndSlot);
			return true;
		}
		// EndSlot 의 ItemType 이 Ammo 이고, StartSlot 의 WeaponType, DamageType 을 가지고 있으면 SwapSlot
		else if (EndSlot->ItemData.ItemType == EItemType::Ammo && StartSlot->HasTag(EndWeaponType) && StartSlot->
			HasTag(EndDamageType))
		{
			SwapSlot(StartSlot, EndSlot);
			return true;
		}
		else
		{
			return false;
		}
	}

	if (EndSlot->HasTag("WeaponSlot"))
	{
		// StartSlot 은 비어있을 수 없으나, 일단 구현
		if (StartSlot->IsEmpty())
		{
			SwapSlot(StartSlot, EndSlot);
			return true;
		}
		// StartSlot 의 ItemType 이 Ammo 이고, EndSlot 의 WeaponType, DamageType 을 가지고 있으면 SwapSlot
		else if (StartSlot->ItemData.ItemType == EItemType::Ammo && EndSlot->HasTag(StartWeaponType) && EndSlot->
			HasTag(StartDamageType))
		{
			SwapSlot(StartSlot, EndSlot);
			return true;
		}
		else
		{
			return false;
		}
	}

	SwapSlot(StartSlot, EndSlot);

	// // 놓은 지점이 WeaponSlot 이고,
	// if (EndSlot->HasTag("WeaponSlot"))
	// {
	// 	// StartSlot 이 Ammo 아이템이고, EndSlot 이 StartSlot 의 WeaponType 을 가지고 있으면 SwapSlot
	// 	auto* StartAmmoData = static_cast<FAmmoData*>(StartSlot->PickableData.Get());
	//
	// 	if (StartSlot->ItemData.ItemType == EItemType::Ammo &&
	// 		EndSlot->HasTag(UEnum::GetValueAsName(StartAmmoData->WeaponType)))
	// 	{
	// 		SwapSlot(StartSlot, EndSlot);
	// 	}
	// 	else
	// 	{
	// 		return false;
	// 	}
	// }
	// else
	// {
	// 	SwapSlot(StartSlot, EndSlot);
	// }

	// StartSlot 이나 EndSlot 둘 중 하나가 "Equip" 태그를 가지고 있으면, OnEquipDropItem 를 Execute
	// if (StartSlot->HasTag("WeaponSlot"))
	// {
	// 	StartSlot->OnEquipDropItem.ExecuteIfBound();
	// }
	// else if (EndSlot->HasTag("WeaponSlot"))
	// {
	// 	EndSlot->OnEquipDropItem.ExecuteIfBound();
	// }


	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}

void UItemSlot::SetItemData(const FItemInstanceData& InItemData, TSharedPtr<FPickableData> InPickableData)
{
	// ItemName 을 ItemData 의 ItemName 으로 설정
	ItemName = FName(InItemData.ItemName.ToString());
	ItemData = InItemData;

	// PickableData
	PickableData = InPickableData;

	// todo: deprecated
	ParentName = FName("Inventory");
	// AddTag("InventorySlot");

	// ItemData 의 ItemThumbnail 을 Image_InventorySlot 의 Brush 로 설정
	if (Image_InventorySlot)
	{
		Image_InventorySlot->SetBrushFromTexture(InItemData.ItemThumbnail);
		ItemThumbnail = InItemData.ItemThumbnail;
	}

	///////////////////////////
	/// ItemData.bStackable ///
	///////////////////////////

	// ItemData 의 bStackable 이 true 이면
	if (ItemData.bStackable)
	{
		// Text_ItemAmount 의 Visibility 를 Visible 로 설정
		// Text_ItemAmount 의 Text 를 ItemData 의 StackCount 로 설정
		if (Text_ItemAmount)
		{
			Text_ItemAmount->SetVisibility(ESlateVisibility::Visible);
			Text_ItemAmount->SetText(FText::FromString(FString::FromInt(ItemData.MaxStackCount)));
		}
	}
	else
	{
		// Text_ItemAmount 의 Visibility 를 Hidden 로 설정
		if (Text_ItemAmount)
		{
			Text_ItemAmount->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	/////////////////////////////////////////////
	/// ItemData.EItemType == EItemType::Ammo ///
	/////////////////////////////////////////////

	if (ItemData.ItemType == EItemType::Ammo)
	{
		// Text_AmmoAmount 의 Visibility 를 Visible 로 설정
		// Text_AmmoAmount 의 Text 를 ItemData 의 AmmoData.AmmoCount 로 설정

		FAmmoData* AmmoData = GetAmmoData();

		if (Text_AmmoAmount)
		{
			SetAmmoAmount(AmmoData->AmmoCount);
			Text_AmmoAmount->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		// Text_AmmoAmount 의 Visibility 를 Hidden 로 설정
		if (Text_AmmoAmount)
		{
			Text_AmmoAmount->SetVisibility(ESlateVisibility::Hidden);
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

void UItemSlot::SetAmmoAmount(const int32 AmmoAmount)
{
	if (Text_AmmoAmount)
	{
		Text_AmmoAmount->SetVisibility(ESlateVisibility::Visible);
		Text_AmmoAmount->SetText(FText::FromString(FString::FromInt(AmmoAmount)));
	}
}

void UItemSlot::ClearItemSlot()
{
	// ParentName 을 제외한 ItemSlot 의 ItemName 을 초기화
	ItemName = FName();
	ItemData = FItemInstanceData();
	PickableData.Reset();
	ItemThumbnail = BasicTexture;
	Image_InventorySlot->SetBrushFromTexture(BasicTexture);

	// Border_ItemAmount 의 Visibility 를 Hidden 로 설정
	if (Text_ItemAmount)
	{
		Text_ItemAmount->SetVisibility(ESlateVisibility::Hidden);
	}

	// Border_AmmoAmount 의 Visibility 를 Hidden 로 설정
	if (Text_AmmoAmount)
	{
		Text_AmmoAmount->SetVisibility(ESlateVisibility::Hidden);
	}

	// Text_ItemAmount 의 text 를 초기화
	if (Text_ItemAmount)
	{
		Text_ItemAmount->SetText(FText::FromString(""));
	}

	// Text_AmmoAmount 의 text 를 초기화
	if (Text_AmmoAmount)
	{
		Text_AmmoAmount->SetText(FText::FromString(""));
	}
}

bool UItemSlot::IsEmpty() const
{
	return ItemName.IsNone();
}

void UItemSlot::SetCheckImage(const FLinearColor& rgba)
{
	CheckImage->SetColorAndOpacity(rgba);
}

FAmmoData* UItemSlot::GetAmmoData()
{
	if (PickableData.IsValid())
	{
		FAmmoData* AmmoData = static_cast<FAmmoData*>(PickableData.Get());
		return AmmoData;
	}
	else
	{
		// UE_LOG(LogTemp, Error, TEXT("PickableData is not valid"));
		return nullptr;
	}
}

// todo: 지금 사용중이지 않음
void UItemSlot::TransferSlot(UItemSlot* SourceSlot, UItemSlot* TargetSlot)
{
	if (!SourceSlot || !TargetSlot)
	{
		UE_LOG(LogTemp, Warning, TEXT("Invalid Slot(s) provided for TransferSlot"));
		return;
	}

	// SourceSlot의 ItemName, ItemInstanceData, ItemThumbnail을 TargetSlot에 설정
	TargetSlot->ItemName = SourceSlot->ItemName;
	TargetSlot->ItemData = SourceSlot->ItemData;
	TargetSlot->ItemThumbnail = SourceSlot->ItemThumbnail;

	// TargetSlot의 이미지 업데이트
	TargetSlot->SetItemImage(SourceSlot->ItemThumbnail);
}

void UItemSlot::SwapSlot(UItemSlot* SlotA, UItemSlot* SlotB)
{
	if (!SlotA || !SlotB)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Invalid Slot(s) provided for SwapSlot"));
		return;
	}
	// UE_LOG(LogTemp, Warning, TEXT("UItemSlot::SwapSlot"));
	// // SlotA와 SlotB의 ItemName 출력
	// UE_LOG(LogTemp, Warning, TEXT("SlotA: %s, SlotB: %s"), *SlotA->ItemName.ToString(), *SlotB->ItemName.ToString());

	// ItemName Swap
	Swap(SlotA->ItemName, SlotB->ItemName);

	// ItemThumbnail Swap
	Swap(SlotA->ItemThumbnail, SlotB->ItemThumbnail);
	SlotA->SetItemImage(SlotA->ItemThumbnail);
	SlotB->SetItemImage(SlotB->ItemThumbnail);


	// ItemInstanceData Swap
	Swap(SlotA->ItemData, SlotB->ItemData);

	// PickableData Swap
	Swap(SlotA->PickableData, SlotB->PickableData);

	// 두 위젯 중 하나가 Ammo 아이템이면, AmmoAmount 업데이트
	if (SlotA->ItemData.ItemType == EItemType::Ammo || SlotB->ItemData.ItemType == EItemType::Ammo)
	{
		FAmmoData* AmmoDataA = SlotA->GetAmmoData();
		FAmmoData* AmmoDataB = SlotB->GetAmmoData();

		int32 CountA = AmmoDataA ? AmmoDataA->AmmoCount : 0;
		int32 CountB = AmmoDataB ? AmmoDataB->AmmoCount : 0;

		SlotA->SetAmmoAmount(CountA);
		SlotB->SetAmmoAmount(CountB);

		if (CountA > 0)
		{
			SlotA->Text_AmmoAmount->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			SlotA->Text_AmmoAmount->SetVisibility(ESlateVisibility::Hidden);
		}

		if (CountB > 0)
		{
			SlotB->Text_AmmoAmount->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			SlotB->Text_AmmoAmount->SetVisibility(ESlateVisibility::Hidden);
		}

		// StartSlot 이나 EndSlot 둘 중 하나가 "Equip" 태그를 가지고 있으면, OnEquipDropItem 를 Execute
		if (SlotA->HasTag("WeaponSlot") && !SlotA->OnEquipDropItem.ExecuteIfBound())
		{
			UE_LOG(LogTemp, Error, TEXT("SlotA->OnEquipDropItem.ExecuteIfBound() is false"));
		}
		else if (SlotB->HasTag("WeaponSlot") && !SlotB->OnEquipDropItem.ExecuteIfBound())
		{
			UE_LOG(LogTemp, Error, TEXT("SlotB->OnEquipDropItem.ExecuteIfBound() is false"));
		}
	}

	// slotA 가 Stackable 이면, ItemAmount 업데이트
	// 또한 Border_ItemAmount 의 Visibility 를 Visible 로 설정
	if (SlotA->ItemData.bStackable)
	{
		SlotA->Text_ItemAmount->SetVisibility(ESlateVisibility::Visible);
		SlotA->Text_ItemAmount->SetText(FText::FromString(FString::FromInt(SlotA->ItemData.MaxStackCount)));
	}
	else
	{
		SlotA->Text_ItemAmount->SetVisibility(ESlateVisibility::Hidden);
	}

	// slotB 가 Stackable 이면, ItemAmount 업데이트
	// 또한 Border_ItemAmount 의 Visibility 를 Visible 로 설정
	if (SlotB->ItemData.bStackable)
	{
		SlotB->Text_ItemAmount->SetVisibility(ESlateVisibility::Visible);
		SlotB->Text_ItemAmount->SetText(FText::FromString(FString::FromInt(SlotB->ItemData.MaxStackCount)));
	}
	else
	{
		SlotB->Text_ItemAmount->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UItemSlot::OnButtonClicked()
{
	// todo:
	// OnItemSlotClicked.ExecuteIfBound(ItemName);

	// debug this name
	UE_LOG(LogTemp, Warning, TEXT("ThisName: %s"), *GetName());
	// debug parent name
	UE_LOG(LogTemp, Warning, TEXT("ParentName: %s"), *ParentName.ToString());
	// debug item name
	UE_LOG(LogTemp, Warning, TEXT("ItemName: %s"), *ItemName.ToString());
	// debug ItemInstanceData.ItemOptionDescription
	UE_LOG(LogTemp, Warning, TEXT("ItemOptionDescription: %s"), *ItemData.ItemOptionDescription);

	// debug Tags
	for (auto& Tag : Tags)
	{
		UE_LOG(LogTemp, Warning, TEXT("Tag: %s"), *Tag.ToString());
	}

	// PickableData 가 nullptr 이 아니면, PickableData 를 FAmmoData 로 캐스팅해서 WeaponType, DamageType 출력
	if (PickableData.IsValid())
	{
		FAmmoData* AmmoData = static_cast<FAmmoData*>(PickableData.Get());
		if (AmmoData)
		{
			UE_LOG(LogTemp, Warning, TEXT("WeaponType: %s, DamageType: %s"),
			       *UEnum::GetValueAsString(AmmoData->WeaponType),
			       *UEnum::GetValueAsString(AmmoData->DamageType));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AmmoData is nullptr"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("PickableData is nullptr"));
	}
}
