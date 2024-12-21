// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "UI/Equip/EquipWidget.h"


// Sets default values for this component's properties
UEquipComponent::UEquipComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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
			}
		}
	}
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
