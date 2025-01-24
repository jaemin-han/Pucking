// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipComponent.h"

#include "AnimComponent.h"
#include "InputTriggers.h"
#include "GunActorComponent.h"
#include "InventoryComponent.h"
#include "Character/PuckingCharacter.h"
#include "GameFramework/Character.h"
#include "UI/Equip/EquipWidget.h"
#include "UI/Equip/WeaponSlot.h"
#include "UI/Inventory/ItemSlot.h"
#include "UI/HUD/MainHUD.h"


// Sets default values for this component's properties
UEquipComponent::UEquipComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// CurWeaponType 을 Rifle 로 초기화
	CurWeaponType = EWeaponType::Rifle;
	// CurAmmoIndex 를 0 으로 초기화
	CurAmmoIndex = 0;

	// WeaponAmmoIndex 에 Rifle, Shotgun 을 -1 으로 초기화
	WeaponAmmoIndexMap.Add(EWeaponType::Rifle, -1);
	WeaponAmmoIndexMap.Add(EWeaponType::Shotgun, -1);
	WeaponAmmoIndexMap.Add(EWeaponType::BFG, -1);
	WeaponAmmoIndexMap.Add(EWeaponType::Hammer, -1);

	// WeaponPickableDataMap 에 모든 무기를 빈 TSharedPtr<FPickableData> 로 초기화
	WeaponPickableDataMap.Add(EWeaponType::Rifle, nullptr);
	WeaponPickableDataMap.Add(EWeaponType::Shotgun, nullptr);
	WeaponPickableDataMap.Add(EWeaponType::BFG, nullptr);
	WeaponPickableDataMap.Add(EWeaponType::Hammer, nullptr);

	// WeaponOptionArrayMap 에 모든 무기를 빈 ItemInstanceData 로 초기화
	WeaponItemDataMap.Add(EWeaponType::Rifle, FItemInstanceData());
	WeaponItemDataMap.Add(EWeaponType::Shotgun, FItemInstanceData());
	WeaponItemDataMap.Add(EWeaponType::BFG, FItemInstanceData());
	WeaponItemDataMap.Add(EWeaponType::Hammer, FItemInstanceData());
}


// Called when the game starts
void UEquipComponent::BeginPlay()
{
	Super::BeginPlay();

	// set owner
	Owner = Cast<ACharacter>(GetOwner());
	OwnerPlayerController = Cast<APlayerController>(Owner->GetController());

	// create EquipWidget
	EquipWidget = CreateWidget<UEquipWidget>(GetWorld(), EquipWidgetClass);
	// bind AddItemSlot to EquipWidget -> WeaponSlot_0 ~ 3 -> OnAddItemSlot
	EquipWidget->WeaponSlot_0->OnAddItemSlot.AddDynamic(this, &UEquipComponent::AddItemSlot);
	EquipWidget->WeaponSlot_1->OnAddItemSlot.AddDynamic(this, &UEquipComponent::AddItemSlot);
	EquipWidget->WeaponSlot_2->OnAddItemSlot.AddDynamic(this, &UEquipComponent::AddItemSlot);
	EquipWidget->WeaponSlot_3->OnAddItemSlot.AddDynamic(this, &UEquipComponent::AddItemSlot);

	// // create MainHUD
	// MainHUD = CreateWidget<UMainHUD>(GetWorld(), MainHUDClass);
	// MainHUD->AddToViewport(0);
	// MainHUD->SetAmmoImageTintRed(CurAmmoIndex);

	// get MainHUD from APuckingCharacter
	APuckingCharacter* PuckingCharacter = Cast<APuckingCharacter>(Owner);
	if (PuckingCharacter)
	{
		MainHUD = PuckingCharacter->MainHUD;
	}

	// get all GunActorComponent
	TArray<UActorComponent*> ActorComponents;
	Owner->GetComponents(ActorComponents);
	for (auto* ActorComponent : ActorComponents)
	{
		UGunActorComponent* GunActorComponent = Cast<UGunActorComponent>(ActorComponent);
		if (GunActorComponent)
		{
			// Initialize GunActorComponent
			GunActorComponent->InitActorComponent();

			// bind OnReload to GunActorComponent -> OnRemainAmmo
			GunActorComponent->OnRemainAmmo.BindUObject(this, &UEquipComponent::OnReload);
			GunActorComponent->OnIsRemainAmmo.BindUObject(this, &UEquipComponent::IsAvailableAmmo);
			GunActorComponent->OnGetDamageType.BindUObject(this, &UEquipComponent::GetDamageType);
		}
	}
	OnWeaponTypeChanged.Broadcast(CurWeaponType);

	// get AnimComponent, AnimComponent 도 CurWeaponType 를 추적할 수 있게
	UAnimComponent* AnimComponent = Owner->FindComponentByClass<UAnimComponent>();
	if (AnimComponent)
	{
		OnWeaponTypeChanged.AddDynamic(AnimComponent, &UAnimComponent::HandleWeaponType);
	}

	// get InventoryComponent
	InventoryComponent = Owner->GetComponentByClass<UInventoryComponent>();
	if (!InventoryComponent)
		UE_LOG(LogTemp, Error, TEXT("InventoryComponent is nullptr"));
	InventoryComponent->OnPickupItem.AddDynamic(this, &UEquipComponent::HandlePickupItem);
}


// Called every frame
void UEquipComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// // GEngine->AddOnScreenDebugMessage 으로 CurWeaponType, CurAmmoIndex 를 출력
	// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red, FString::Printf(TEXT("CurAmmoIndex: %d"), CurAmmoIndex));
	// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red,
	//                                  FString::Printf(
	// 	                                 TEXT("CurWeaponType: %s"), *UEnum::GetValueAsString(CurWeaponType)));
	//
	// // debug WeaponAmmoIndexMap
	// for (auto& Elem : WeaponAmmoIndexMap)
	// {
	// 	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::Red,
	// 	                                 FString::Printf(TEXT("WeaponAmmoIndexMap: %s, %d"),
	// 	                                                 *UEnum::GetValueAsString(Elem.Key), Elem.Value));
	// }
}

void UEquipComponent::HandleWeaponType(const FInputActionValue& Value)
{
	// InputValue 를 int32 로 변환
	float InputValue = Value.Get<float>();

	if (InputValue == 1.0 && bIsRifleActivated)
	{
		CurWeaponType = EWeaponType::Rifle;
	}
	else if (InputValue == 2.0 && bIsShotgunActivated)
	{
		CurWeaponType = EWeaponType::Shotgun;
	}
	else if (InputValue == 3.0 && bIsWeaponTBD1Activated)
	{
		CurWeaponType = EWeaponType::BFG;
	}
	else if (InputValue == 4.0 && bIsWeaponTBD2Activated)
	{
		CurWeaponType = EWeaponType::Hammer;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid WeaponType %f"), InputValue);
		return;
	}


	// 기존에 해당 WeaponType 의 AmmoIndex 를 가져옴
	CurAmmoIndex = WeaponAmmoIndexMap[CurWeaponType];
	// 만약 AmmoIndex 가 -1 이면 0 으로 초기화
	if (CurAmmoIndex == -1)
	{
		CurAmmoIndex = 0;
	}

	// todo: WeaaponComponent 와 통신할 때 사용함
	// delegate 를 호출
	OnWeaponTypeChanged.Broadcast(CurWeaponType);

	// StatusComponent 에 현재 상태를 반영한다
	OnStatusComponentChanged.Broadcast(CurWeaponType, CurAmmoIndex);

	// debug CurWeaponType
	UE_LOG(LogTemp, Warning, TEXT("CurWeaponType: %s"), *UEnum::GetValueAsString(CurWeaponType));
	ApplyToMainHUD();

	// WeaponAmmoIndexMap[CurWeaponType] 는 -1 로 초기화됨으로, 이 떄는
	// 장전되지 않은 상황을 의미하므로 MainHUD 의 SetCurrentMagaineImage 를 호출하지 않음
	if (WeaponAmmoIndexMap[CurWeaponType] != -1)
		MainHUD->SetCurrentMagaineImage(WeaponItemDataMap[CurWeaponType].ItemThumbnail);
	else
	{
		// 장전되지 않은 상황이므로, BasicTexture 를 호출
		MainHUD->SetCurrentMagaineImage(GetItemSlot(CurWeaponType, 0)->BasicTexture);
	}
}

void UEquipComponent::HandleAmmoIndex(const FInputActionValue& Value)
{
	int32 InputValue = static_cast<int32>(Value.Get<float>());

	UE_LOG(LogTemp, Warning, TEXT("InputValue: %d"), InputValue);

	// InputValue 가 1.0 일 때
	if (InputValue == 1)
	{
		CurAmmoIndex = (CurAmmoIndex + 1) % 3;
	}
	// InputValue 가 -1.0 일 때
	else
	{
		CurAmmoIndex = (CurAmmoIndex - 1 + 3) % 3;
	}

	MainHUD->SetAmmoImageTintRed(CurAmmoIndex);


	UE_LOG(LogTemp, Warning, TEXT("CurAmmoIndex: %d"), WeaponAmmoIndexMap[CurWeaponType]);

	// todo: Weapon Component 의 Reload 를 호출해야함
	// todo: 라고 생각했는데, 생각해보니 굳이 Reload 를 호출할 필요가 없음
	// todo: 걍 HUD 에 반영하면 됨
	OnAmmoIndexChanged.Broadcast();
}

void UEquipComponent::HandleEquipOnOff()
{
	UE_LOG(LogTemp, Warning, TEXT("HandleEquipOnOff"));
	// InventoryGrid 가 화면에 보이지 않으면 화면에 보이도록 설정
	if (!EquipWidget->IsInViewport())
	{
		EquipWidget->AddToViewport(1);
		// mode mode 를 UIOnly 로 설정
		OwnerPlayerController->SetInputMode(FInputModeGameAndUI());
		// show mouse cursor
		OwnerPlayerController->bShowMouseCursor = true;
	}
	else
	{
		EquipWidget->RemoveFromParent();
		// mode mode 를 GameOnly 로 설정
		OwnerPlayerController->SetInputMode(FInputModeGameOnly());
		// hide mouse cursor
		OwnerPlayerController->bShowMouseCursor = false;
	}
}

TArray<FInputParameter> UEquipComponent::ReturnInputParameter()
{
	if (EquipMappingContext)
	{
		// InventoryComponent 에서 EquipWidget 을 On/Off 하는 함수를 호출하는 방식으로 수정함
		// if (EquipOnOffAction)
		// {
		// 	FInputParameter EquipOnOffInputParameter;
		//
		// 	EquipOnOffInputParameter.TargetClass = this;
		// 	EquipOnOffInputParameter.TriggerEvent = ETriggerEvent::Started;
		// 	EquipOnOffInputParameter.InputMappingContext = EquipMappingContext;
		// 	EquipOnOffInputParameter.InputAction = EquipOnOffAction;
		// 	EquipOnOffInputParameter.CallbackFunc = FName("HandleEquipOnOff");
		//
		// 	InputParameters.Push(EquipOnOffInputParameter);
		// }

		if (ChangeWeaponTypeAction)
		{
			FInputParameter ChangeWeaponTypeInputParameter;

			ChangeWeaponTypeInputParameter.TargetClass = this;
			ChangeWeaponTypeInputParameter.TriggerEvent = ETriggerEvent::Started;
			ChangeWeaponTypeInputParameter.InputMappingContext = EquipMappingContext;
			ChangeWeaponTypeInputParameter.InputAction = ChangeWeaponTypeAction;
			ChangeWeaponTypeInputParameter.CallbackFunc = FName("HandleWeaponType");

			InputParameters.Push(ChangeWeaponTypeInputParameter);
		}

		if (ChangeAmmoIndexAction)
		{
			FInputParameter ChangeAmmoIndexInputParameter;

			ChangeAmmoIndexInputParameter.TargetClass = this;
			ChangeAmmoIndexInputParameter.TriggerEvent = ETriggerEvent::Started;
			ChangeAmmoIndexInputParameter.InputMappingContext = EquipMappingContext;
			ChangeAmmoIndexInputParameter.InputAction = ChangeAmmoIndexAction;
			ChangeAmmoIndexInputParameter.CallbackFunc = FName("HandleAmmoIndex");

			InputParameters.Push(ChangeAmmoIndexInputParameter);
		}
	}
	return InputParameters;
}

void UEquipComponent::AddItemSlot(EWeaponType InWeaponType, class UItemSlot* ItemSlot)
{
	WeaponItemSlotMap.FindOrAdd(InWeaponType).ItemSlots.Add(ItemSlot);
	ItemSlot->OnEquipDropItem.BindDynamic(this, &UEquipComponent::ApplyToMainHUD);
}


int32 UEquipComponent::OnReload(int32 MagazineCapacity)
{
	// WeaponItemSlots 의 WeaponType 에 해당하는 FItemSlotArray 를 찾아서 ItemSlots 에 접근
	auto& ItemSlots = WeaponItemSlotMap[CurWeaponType].ItemSlots;

	// CurAmmoIndex 에 해당하는 ItemSlot 의 ItemInstanceData 의 Ammo 를 가져옴
	if (ItemSlots.IsValidIndex(CurAmmoIndex))
	{
		auto* ItemSlot = ItemSlots[CurAmmoIndex];
		auto* AmmoData = ItemSlot->GetAmmoData();

		// WeaponPickableDataMap
		WeaponPickableDataMap[CurWeaponType] = ItemSlot->PickableData;
		WeaponItemDataMap[CurWeaponType] = ItemSlot->ItemData;

		if (!AmmoData)
		{
			UE_LOG(LogTemp, Error, TEXT("AmmoData is not valid"));
			return 0;
		}

		int32 ReturnValue;
		int32 RemainingAmmo = AmmoData->AmmoCount;

		UTexture2D* AmmoImage = GetItemSlot(CurWeaponType, CurAmmoIndex)->ItemThumbnail;
		if (RemainingAmmo > MagazineCapacity)
		{
			// todo: 사용된 Ammo 를 UI 에 반영해야함
			RemainingAmmo -= MagazineCapacity;
			AmmoData->AmmoCount = RemainingAmmo;
			ItemSlot->SetAmmoAmount(RemainingAmmo);
			ReturnValue = MagazineCapacity;
		}
		else
		{
			// ItemSlot->ItemInstanceData.AmmoData.AmmoCount = 0;
			// ItemSlot->SetAmmoAmount(0);
			ItemSlots[CurAmmoIndex]->ClearItemSlot();

			SwapValidAmmo();

			ReturnValue = RemainingAmmo;
		}

		WeaponAmmoIndexMap[CurWeaponType] = CurAmmoIndex;

		// StatusComponent 에 현재 상태를 반영한다
		OnStatusComponentChanged.Broadcast(CurWeaponType, CurAmmoIndex);

		// MainHUD 의 SetCurrentMagazineImage 를 호출
		ApplyToMainHUD();
		MainHUD->SetCurrentMagaineImage(AmmoImage);
		return ReturnValue;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AmmoIndex %d is not found"), CurAmmoIndex);
		return -1;
	}
}

bool UEquipComponent::IsAvailableAmmo(int32 MagazineCapacity)
{
	// WeaponItemSlots 의 WeaponType 에 해당하는 FItemSlotArray 를 찾아서 ItemSlots 에 접근

	// WeaponItemSlotMap 에서 CurWeaponType 가 유효한지 체크
	if (!WeaponItemSlotMap.Contains(CurWeaponType))
	{
		UE_LOG(LogTemp, Error, TEXT("CurWeaponType %s is not found"), *UEnum::GetValueAsString(CurWeaponType));
		return false;
	}
	auto& ItemSlots = WeaponItemSlotMap[CurWeaponType].ItemSlots;
	auto* ItemSlot = ItemSlots[CurAmmoIndex];
	auto* AmmoData = ItemSlot->GetAmmoData();


	// CurAmmoIndex 에 해당하는 ItemSlot 의 ItemInstanceData 의 Ammo 를 가져옴
	int32 RemainingAmmo = AmmoData ? AmmoData->AmmoCount : 0;
	if (RemainingAmmo > 0)
		return true;
	else
	{
		if (SwapValidAmmo())
			return IsAvailableAmmo(MagazineCapacity);
		else
			return false;
	}
}

EDamageType UEquipComponent::GetDamageType()
{
	// todo: 현재 Ammo 타입 저장하는 로직 바뀌면 수정되야 할 수도 있음
	// CurWeaponType, CurAmmoIndex 에 해당하는 ItemSlot 가져오기
	// CurWeaponType, CurAmmoIndex debug
	UE_LOG(LogTemp, Warning, TEXT("CurWeaponType: %s"), *UEnum::GetValueAsString(CurWeaponType));
	UE_LOG(LogTemp, Warning, TEXT("CurAmmoIndex: %d"), CurAmmoIndex);
	auto* ItemSlot = GetItemSlot(CurWeaponType, CurAmmoIndex);
	if (!ItemSlot)
	{
		UE_LOG(LogTemp, Error, TEXT("ItemSlot is nullptr"));
		return EDamageType::Fire;
	}
	else if (!ItemSlot->GetAmmoData())
	{
		UE_LOG(LogTemp, Error, TEXT("AmmoData is nullptr"));
		return EDamageType::Fire;
	}
	// ItemSlot 의 AmmoData 의 DamageType 을 리턴
	return ItemSlot->GetAmmoData()->DamageType;
}

bool UEquipComponent::SwapValidAmmo()
{
	if (InventoryComponent)
	{
		auto* NewItemSlot = InventoryComponent->GetFirstAmmoItemSlot(CurWeaponType,
		                                                             static_cast<EDamageType>(CurAmmoIndex));
		if (NewItemSlot)
		{
			auto* CurrentAmmoSlot = GetItemSlot(CurWeaponType, CurAmmoIndex);
			if (CurrentAmmoSlot)
			{
				// SwapSlot
				UItemSlot::SwapSlot(NewItemSlot, CurrentAmmoSlot);
				ApplyToMainHUD();
				return true;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("CurrentAmmoSlot is nullptr"));
				return false;
			}
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("NewItemSlot is nullptr"));
			return false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryComponent is nullptr"));
		return false;
	}
}

void UEquipComponent::HandlePickupItem(UItemSlot* ItemSlot)
{
	if (ItemSlot->IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("ItemSlot is Empty"));
		return;
	}

	// Ammo 아이템이면,
	if (ItemSlot->ItemData.ItemType == EItemType::Ammo)
	{
		auto* AmmoData = ItemSlot->GetAmmoData();
		if (!AmmoData)
		{
			UE_LOG(LogTemp, Error, TEXT("AmmoData is not valid"));
			return;
		}

		// 해당 아이템과 맞는 WeaponItemSlot 이 비어있으면 SwapSlot
		auto* WeaponItemSlot = GetItemSlot(AmmoData->WeaponType, static_cast<int32>(AmmoData->DamageType));
		if (WeaponItemSlot->IsEmpty())
		{
			UItemSlot::SwapSlot(ItemSlot, WeaponItemSlot);
		}
	}
}

class UItemSlot* UEquipComponent::GetItemSlot(EWeaponType InWeaponType, int32 InAmmoIndex)
{
	// WeaponItemSlots 의 WeaponType 에 해당하는 FItemSlotArray 를 찾아서 ItemSlots 에 접근
	if (WeaponItemSlotMap.Contains(InWeaponType))
	{
		auto& ItemSlots = WeaponItemSlotMap[InWeaponType].ItemSlots;
		if (ItemSlots.IsValidIndex(InAmmoIndex))
		{
			return ItemSlots[InAmmoIndex];
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AmmoIndex %d is not found"), InAmmoIndex);
			return nullptr;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponType %d is not found"), InWeaponType);
		return nullptr;
	}
}

TArray<class UOptionDataAsset*> UEquipComponent::GetItemOptions(EWeaponType InWeaponType, int32 InAmmoIndex)
{
	// // WeaponItemSlots 의 WeaponType 에 해당하는 FItemSlotArray 를 찾아서 ItemSlots 에 접근
	// if (WeaponItemSlotMap.Contains(InWeaponType))
	// {
	// 	auto& ItemSlots = WeaponItemSlotMap[InWeaponType].ItemSlots;
	// 	if (ItemSlots.IsValidIndex(InAmmoIndex))
	// 	{
	// 		return ItemSlots[InAmmoIndex]->ItemData.ItemOptions;
	// 	}
	// 	else
	// 	{
	// 		UE_LOG(LogTemp, Error, TEXT("AmmoIndex %d is not found"), InAmmoIndex);
	// 		return TArray<class UOptionDataAsset*>();
	// 	}
	// }
	// else
	// {
	// 	UE_LOG(LogTemp, Error, TEXT("WeaponType %d is not found"), InWeaponType);
	// 	return TArray<class UOptionDataAsset*>();
	// }

	// WeaponOptionArrayMap 에서 InWeaponType 에 해당하는 TArray<class UOptionDataAsset*> 를 리턴
	if (WeaponItemDataMap.Contains(InWeaponType))
	{
		return WeaponItemDataMap[InWeaponType].ItemOptions;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponType %d is not found"), InWeaponType);
		return TArray<UOptionDataAsset*>();
	}
}

void UEquipComponent::ApplyToMainHUD()
{
	// Apply WeaponSlot to HUD
	switch (CurWeaponType)
	{
	case EWeaponType::Rifle:
		MainHUD->ApplyWeaponSlotToHUD(EquipWidget->WeaponSlot_0);
		break;
	case EWeaponType::Shotgun:
		MainHUD->ApplyWeaponSlotToHUD(EquipWidget->WeaponSlot_1);
		break;
	case EWeaponType::BFG:
		MainHUD->ApplyWeaponSlotToHUD(EquipWidget->WeaponSlot_2);
		break;
	case EWeaponType::Hammer:
		MainHUD->ApplyWeaponSlotToHUD(EquipWidget->WeaponSlot_3);
		break;
	default:
		break;
	}

	// Apply AmmoIndex to HUD
	MainHUD->SetAmmoImageTintRed(CurAmmoIndex);
}
