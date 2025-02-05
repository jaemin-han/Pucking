// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "EquipComponent.h"
#include "InputTriggers.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Character/PuckingCharacter.h"
#include "Common/CommonStruct.h"
#include "Components/Button.h"
#include "GameFramework/Character.h"
#include "Item/PickableItem.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Inventory/InventoryGrid.h"
#include "UI/Inventory/ItemSlot.h"


class UEnhancedInputLocalPlayerSubsystem;
// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ACharacter>(GetOwner());
	OwnerPlayerController = Cast<APlayerController>(Owner->GetController());
	OwnerCameraComponent = Owner->FindComponentByClass<UCameraComponent>();

	// DetectInteractingItem 함수가 일정 주기로 호출되도록 설정
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UInventoryComponent::DetectInteractingItem, 0.1f, true);

	// InventoryGridClass 를 사용해서 InventoryGrid 를 생성
	InventoryGrid = CreateWidget<UInventoryGrid>(GetWorld(), InventoryGridClass);

	// Create 45 item slots
	for (int32 i = 0; i < InventoryGrid->MaxSlotCount; ++i)
	{
		auto* ItemSlot = CreateWidget<UItemSlot>(GetWorld(), ItemSlotClass);
		ItemSlot->AddTag("InventorySlot");
		// bind ItemSlotClicked
		InventoryGrid->AddItemSlot(ItemSlot);
		// ItemSlotArray 에 ItemSlot 추가
		ItemSlotArray.Add(ItemSlot);
	}

	// EquipComponent 가져오기
	UEquipComponent* EquipComponent = Owner->FindComponentByClass<UEquipComponent>();
	// 서로의 delegate 를 bind
	if (EquipComponent)
	{
		InventoryOnOffDelegate.BindUObject(EquipComponent, &UEquipComponent::HandleEquipOnOff);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("EquipComponent is not found"));
	}

	// Owner 로부터 APuckingCharacter 가져오기
	APuckingCharacter* PuckingCharacter = Cast<APuckingCharacter>(Owner);
	if (PuckingCharacter)
	{
		PuckingCharacter->InventoryGrid = InventoryGrid;
		PuckingCharacter->WidgetSet.Add(InventoryGrid);
	}
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// drawdebugstring
	// InteractingItem
	FString InteractingItemString = InteractingItem ? InteractingItem->GetName() : TEXT("None");
	DrawDebugString(GetWorld(), Owner->GetActorLocation(), InteractingItemString, Owner, FColor::Red, DeltaTime);

	// gengine->addonmessage
	//GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Red, InteractingItemString);
}

TArray<struct FInputParameter> UInventoryComponent::ReturnInputParameter()
{
	if (ItemMappingContext)
	{
		if (ItemInteractionAction)
		{
			FInputParameter ItemInteractionInputParameter;

			ItemInteractionInputParameter.TargetClass = this;
			ItemInteractionInputParameter.TriggerEvent = ETriggerEvent::Started;
			ItemInteractionInputParameter.InputMappingContext = ItemMappingContext;
			ItemInteractionInputParameter.InputAction = ItemInteractionAction;
			ItemInteractionInputParameter.CallbackFunc = FName("HandleInteractingItem");

			InputParameters.Push(ItemInteractionInputParameter);
		}
	}

	return InputParameters;
}

void UInventoryComponent::HandleInteractingItem()
{
	if (InteractingItem)
	{
		// ItemSlotArray 에서 처음으로 ItemName 이 없는 ItemSlot 을 찾음
		auto* EmptyItemSlot = ItemSlotArray.FindByPredicate([](UItemSlot* ItemSlot)
		{
			return ItemSlot->IsEmpty();
		});
		// ItemSlotArray 에 ItemName 이 없는 ItemSlot 이 없으면 return
		// todo: 가방이 꽉 찼다는 UX 만들기
		if (!EmptyItemSlot)
		{
			return;
		}
		if (PickUpSound)  // 사운드가 설정되어 있는지 확인
		{
			FVector SoundLocation = InteractingItem->GetActorLocation(); // 현재 액터의 위치에서 재생
			UGameplayStatics::PlaySoundAtLocation(this,PickUpSound, SoundLocation);
		}
		(*EmptyItemSlot)->SetItemData(InteractingItem->ItemData, InteractingItem->PickableData);

		// EquipComponent 에서 알아서 처리해주세요
		OnPickupItem.Broadcast(*EmptyItemSlot);


		// InteractingItem 을 제거
		InteractingItem->Destroy();
		InteractingItem = nullptr;
	}
}

void UInventoryComponent::DetectInteractingItem()
{
	// linetrace 를 수행해서 상호작용 가능한 아이템을 감지
	// linetrace 를 수행할 때, Owner 의 위치와 방향을 사용
	// linetrace 를 수행한 결과를 InteractingItem 에 저장

	// CameraComponent 위치를 기준으로 linetrace 를 수행
	FVector Start = OwnerCameraComponent->GetComponentLocation() + OwnerCameraComponent->GetForwardVector() * 100.f;
	FVector End = Start + OwnerCameraComponent->GetForwardVector() * 1000.f;
	// SphereTrace 를 수행
	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(Owner);

	// SphereTrace 를 수행한 결과를 InteractingItem 에 저장
	if (GetWorld()->SweepSingleByChannel(HitResult, Start, End, FQuat::Identity, ECC_GameTraceChannel1,
	                                     FCollisionShape::MakeSphere(50.f), CollisionQueryParams))
	{
		InteractingItem = Cast<APickableItem>(HitResult.GetActor());
		// 충돌 지점에 draw debug circle (green)
		/*if (InteractingItem)
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 50.f, 16, FColor::Green, false, 0.1f);*/
	}
	else
	{
		InteractingItem = nullptr;
	}
}

UItemSlot* UInventoryComponent::GetFirstAmmoItemSlot(EWeaponType WeaponType, EDamageType DamageType)
{
	// ItemSlotArray 를 순회한다
	for (auto* ItemSlot : ItemSlotArray)
	{
		// 해당 ItemSlot 의 ItemData 의 ItemType 이 Ammo 인지 확인한다
		if (ItemSlot->ItemData.ItemType != EItemType::Ammo)
			continue;

		// 해당 아이템의 AmmoData 를 가져온다
		auto* AmmoData = ItemSlot->GetAmmoData();

		// WeaponType 확인
		if (AmmoData && AmmoData->WeaponType == WeaponType && AmmoData->DamageType == DamageType)
			return ItemSlot;
	}
	// UE_LOG(LogTemp, Warning, TEXT("%s, %s no valid"), *UEnum::GetValueAsString(WeaponType),
	//        *UEnum::GetValueAsString(DamageType));
	return nullptr;
}
