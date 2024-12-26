// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "UI/Equip/EquipWidget.h"
#include "UI/Equip/WeaponSlot.h"
#include "UI/Inventory/ItemSlot.h"


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

	// WeaponAmmoIndex 에 Rifle, Shotgun 을 0 으로 초기화
	WeaponAmmoIndexMap.Add(EWeaponType::Rifle, 0);
	WeaponAmmoIndexMap.Add(EWeaponType::Shotgun, 0);
	WeaponAmmoIndexMap.Add(EWeaponType::WeaponTBD1, 0);
	WeaponAmmoIndexMap.Add(EWeaponType::WeaponTBD2, 0);
}


// Called when the game starts
void UEquipComponent::BeginPlay()
{
	Super::BeginPlay();

	// set owner
	Owner = Cast<ACharacter>(GetOwner());
	OwnerPlayerController = Cast<APlayerController>(Owner->GetController());
	SetEnhancedInput();

	// create EquipWidget
	EquipWidget = CreateWidget<UEquipWidget>(GetWorld(), EquipWidgetClass);

	// bind AddItemSlot to EquipWidget -> WeaponSlot_0 ~ 3 -> OnAddItemSlot
	EquipWidget->WeaponSlot_0->OnAddItemSlot.AddDynamic(this, &UEquipComponent::AddItemSlot);
	EquipWidget->WeaponSlot_1->OnAddItemSlot.AddDynamic(this, &UEquipComponent::AddItemSlot);
	EquipWidget->WeaponSlot_2->OnAddItemSlot.AddDynamic(this, &UEquipComponent::AddItemSlot);
	EquipWidget->WeaponSlot_3->OnAddItemSlot.AddDynamic(this, &UEquipComponent::AddItemSlot);
}


// Called every frame
void UEquipComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipComponent::SetEnhancedInput()
{
	if (Owner)
	{
		auto* PlayerController = Cast<APlayerController>(Owner->GetController());
		if (PlayerController)
		{
			auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
				PlayerController->GetLocalPlayer());
			if (Subsystem)
			{
				// Mapping Context 의 우선순위를 1로 설정해, Owner 의 MappingContext 보다 우선순위가 높게 설정
				Subsystem->AddMappingContext(EquipMappingContext, 1);
			}

			auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
			if (EnhancedInputComponent)
			{
				// EquipOnOffAction 을 처리하는 함수를 binding
				EnhancedInputComponent->BindAction(EquipOnOffAction, ETriggerEvent::Started, this,
				                                   &UEquipComponent::HandleEquipOnOff);
				// ChangeWeaponTypeAction 을 처리하는 함수를 binding
				EnhancedInputComponent->BindAction(ChangeWeaponTypeAction, ETriggerEvent::Started, this,
				                                   &UEquipComponent::HandleWeaponType);
				// ChangeAmmoIndexAction 을 처리하는 함수를 binding
				EnhancedInputComponent->BindAction(ChangeAmmoIndexAction, ETriggerEvent::Started, this,
				                                   &UEquipComponent::HandleAmmoIndex);
			}
		}
	}
}

void UEquipComponent::HandleWeaponType(const FInputActionValue& Value)
{
	// InputValue 를 int32 로 변환
	float InputValue = Value.Get<float>();

	if (InputValue == 1.0)
	{
		CurWeaponType = EWeaponType::Rifle;
	}
	else if (InputValue == 2.0)
	{
		CurWeaponType = EWeaponType::Shotgun;
	}
	else if (InputValue == 3.0)
	{
		CurWeaponType = EWeaponType::WeaponTBD1;
	}
	else if (InputValue == 4.0)
	{
		CurWeaponType = EWeaponType::WeaponTBD2;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Invalid WeaponType %f"), InputValue);
	}

	// 기존에 해당 WeaponType 의 AmmoIndex 를 가져옴
	CurAmmoIndex = WeaponAmmoIndexMap[CurWeaponType];

	// delegate 를 호출
	OnWeaponTypeChanged.Broadcast(CurWeaponType);

	// debug CurWeaponType
	UE_LOG(LogTemp, Warning, TEXT("CurWeaponType: %s"), *UEnum::GetValueAsString(CurWeaponType));
}

void UEquipComponent::HandleAmmoIndex(const FInputActionValue& Value)
{
	int32 InputValue = static_cast<int32>(Value.Get<float>());

	UE_LOG(LogTemp, Warning, TEXT("InputValue: %d"), InputValue);

	// InputValue 가 1.0 일 때
	if (InputValue == 1)
	{
		WeaponAmmoIndexMap[CurWeaponType] = (WeaponAmmoIndexMap[CurWeaponType] + InputValue) % 3;
	}
	else
	{
		// InputValue 가 -1.0 일 때
		WeaponAmmoIndexMap[CurWeaponType] = (WeaponAmmoIndexMap[CurWeaponType] - InputValue);
		if (WeaponAmmoIndexMap[CurWeaponType] < 0)
		{
			WeaponAmmoIndexMap[CurWeaponType] += 3;
		}
		WeaponAmmoIndexMap[CurWeaponType] %= 3;
	}


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
		EquipWidget->AddToViewport();
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

void UEquipComponent::AddItemSlot(EWeaponType InWeaponType, class UItemSlot* ItemSlot)
{
	WeaponItemSlotMap.FindOrAdd(InWeaponType).ItemSlots.Add(ItemSlot);
}


int32 UEquipComponent::GetRemainingAmmo(int32 MagazineCapacity)
{
	// WeaponItemSlots 의 WeaponType 에 해당하는 FItemSlotArray 를 찾아서 ItemSlots 에 접근
	auto& ItemSlots = WeaponItemSlotMap[CurWeaponType].ItemSlots;
	// CurAmmoIndex 에 해당하는 ItemSlot 의 ItemInstanceData 의 Ammo 를 가져옴
	if (ItemSlots.IsValidIndex(CurAmmoIndex))
	{
		int32 RemainingAmmo = ItemSlots[CurAmmoIndex]->ItemInstanceData.AmmoData.AmmoCount;
		if (RemainingAmmo >= MagazineCapacity)
		{
			// todo: 사용된 Ammo 를 UI 에 반영해야함
			RemainingAmmo -= MagazineCapacity;
			ItemSlots[CurAmmoIndex]->ItemInstanceData.AmmoData.AmmoCount = RemainingAmmo;
			ItemSlots[CurAmmoIndex]->SetAmmoAmount(RemainingAmmo);
			return MagazineCapacity;
		}
		else
		{
			ItemSlots[CurAmmoIndex]->ItemInstanceData.AmmoData.AmmoCount = 0;
			ItemSlots[CurAmmoIndex]->SetAmmoAmount(0);
			return RemainingAmmo;
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AmmoIndex %d is not found"), CurAmmoIndex);
		return -1;
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
	// WeaponItemSlots 의 WeaponType 에 해당하는 FItemSlotArray 를 찾아서 ItemSlots 에 접근
	if (WeaponItemSlotMap.Contains(InWeaponType))
	{
		auto& ItemSlots = WeaponItemSlotMap[InWeaponType].ItemSlots;
		if (ItemSlots.IsValidIndex(InAmmoIndex))
		{
			return ItemSlots[InAmmoIndex]->ItemInstanceData.ItemOptions;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AmmoIndex %d is not found"), InAmmoIndex);
			return TArray<class UOptionDataAsset*>();
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("WeaponType %d is not found"), InWeaponType);
		return TArray<class UOptionDataAsset*>();
	}
}
