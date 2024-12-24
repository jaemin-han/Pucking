// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Item/ItemBase.h"
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

	SetEnhancedInput();

	// DetectInteractingItem 함수가 일정 주기로 호출되도록 설정
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UInventoryComponent::DetectInteractingItem, 0.1f, true);

	// InventoryGridClass 를 사용해서 InventoryGrid 를 생성
	InventoryGrid = CreateWidget<UInventoryGrid>(GetWorld(), InventoryGridClass);

	// Create 45 item slots
	for (int32 i = 0; i < InventoryGrid->MaxSlotCount; ++i)
	{
		auto* ItemSlot = CreateWidget<UItemSlot>(GetWorld(), ItemSlotClass);
		// bind ItemSlotClicked
		InventoryGrid->AddItemSlot(ItemSlot);
		// ItemSlotArray 에 ItemSlot 추가
		ItemSlotArray.Add(ItemSlot);
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
	GEngine->AddOnScreenDebugMessage(0, 0.f, FColor::Red, InteractingItemString);
}

void UInventoryComponent::SetEnhancedInput()
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
				Subsystem->AddMappingContext(ItemMappingContext, 1);
			}

			auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
			if (EnhancedInputComponent)
			{
				EnhancedInputComponent->BindAction(ItemInteractionAction, ETriggerEvent::Started, this,
				                                   &UInventoryComponent::HandleInteractingItem);
				// IA_InventoryOnOff 를 처리하는 함수를 binding
				EnhancedInputComponent->BindAction(InventoryOnOffAction, ETriggerEvent::Started, this,
				                                   &UInventoryComponent::HandleInventoryOnOff);
			}
		}
	}
}

void UInventoryComponent::HandleInteractingItem()
{
	UE_LOG(LogTemp, Warning, TEXT("HandleInteractingItem"));
	if (InteractingItem)
	{
		// ItemSlotArray 에서 처음으로 ItemName 이 없는 ItemSlot 을 찾음
		auto* EmptyItemSlot = ItemSlotArray.FindByPredicate([](UItemSlot* ItemSlot)
		{
			return ItemSlot->ItemName.IsNone();
		});
		// ItemSlotArray 에 ItemName 이 없는 ItemSlot 이 없으면 return
		// todo: 가방이 꽉 찼다는 UX 만들기
		if (!EmptyItemSlot)
		{
			return;
		}

		// EmptyItemSlot 의 ItemName 을 InteractingItem 의 ItemData 의 ItemName 으로 설정
		(*EmptyItemSlot)->ItemName = FName(InteractingItem->ItemData.ItemName.ToString());
		// EmptyItemSlot 의 ItemInstanceData 를 InteractingItem 의 ItemData 로 설정
		(*EmptyItemSlot)->ItemInstanceData = InteractingItem->ItemData;
		// EmptyItemSlot 의 ItemThumbnail 을 InteractingItem 의 ItemData 의 ItemThumbnail 으로 설정
		(*EmptyItemSlot)->ItemThumbnail = InteractingItem->ItemData.ItemThumbnail;
		// EmptyItemSlot 의 이미지 업데이트
		(*EmptyItemSlot)->SetItemImage(InteractingItem->ItemData.ItemThumbnail);


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
		InteractingItem = Cast<AItemBase>(HitResult.GetActor());
		// 충돌 지점에 draw debug circle (green)
		if (InteractingItem)
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 50.f, 16, FColor::Green, false, 0.1f);
	}
	else
	{
		InteractingItem = nullptr;
	}
}

void UInventoryComponent::HandleInventoryOnOff()
{
	UE_LOG(LogTemp, Warning, TEXT("HandleInventoryOnOff"));
	// InventoryGrid 가 화면에 보이지 않으면 화면에 보이도록 설정
	if (!InventoryGrid->IsInViewport())
	{
		InventoryGrid->AddToViewport();
		// mode mode 를 UIOnly 로 설정
		OwnerPlayerController->SetInputMode(FInputModeGameAndUI());
		// show mouse cursor
		OwnerPlayerController->bShowMouseCursor = true;
	}
	else
	{
		InventoryGrid->RemoveFromParent();
		// mode mode 를 GameOnly 로 설정
		OwnerPlayerController->SetInputMode(FInputModeGameOnly());
		// hide mouse cursor
		OwnerPlayerController->bShowMouseCursor = false;
	}
}
