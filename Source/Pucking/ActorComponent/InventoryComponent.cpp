// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Item/ItemBase.h"
#include "Kismet/KismetSystemLibrary.h"


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

	SetEnhancedInput();

	// Owner 의 UCameraComponent 를 찾아서 변수로 저장
	InvenCameraComponent = Owner->FindComponentByClass<UCameraComponent>();

	// DetectInteractingItem 함수가 일정 주기로 호출되도록 설정
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UInventoryComponent::DetectInteractingItem, 0.1f, true);
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
	Owner = Cast<ACharacter>(GetOwner());
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
			}
		}
	}
}

void UInventoryComponent::HandleInteractingItem()
{
	UE_LOG(LogTemp, Warning, TEXT("HandleInteractingItem"));
}

void UInventoryComponent::DetectInteractingItem()
{
	// linetrace 를 수행해서 상호작용 가능한 아이템을 감지
	// linetrace 를 수행할 때, Owner 의 위치와 방향을 사용
	// linetrace 를 수행한 결과를 InteractingItem 에 저장

	// CameraComponent 위치를 기준으로 linetrace 를 수행
	FVector Start = InvenCameraComponent->GetComponentLocation() + InvenCameraComponent->GetForwardVector() * 100.f;
	FVector End = Start + InvenCameraComponent->GetForwardVector() * 1000.f;
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
