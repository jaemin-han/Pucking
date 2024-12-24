// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/StatusComponent.h"

#include "ActorComponent/EquipComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "UI/Status/PlayerStatusWidget.h"


// Sets default values for this component's properties
UStatusComponent::UStatusComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ACharacter>(GetOwner());
	OwnerPlayerController = Cast<APlayerController>(Owner->GetController());
	EquipComp = Owner->FindComponentByClass<UEquipComponent>();

	
	SetEnhancedInput();
	// ...
	
}


// Called every frame
void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void UStatusComponent::SetEnhancedInput()
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
				Subsystem->AddMappingContext(StatusMappingContext, 1);
			}
			auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
			if (EnhancedInputComponent)
			{
				EnhancedInputComponent->BindAction(StatusOnOffAction, ETriggerEvent::Started, this,
					&UStatusComponent::StatusOnOff);
			}

		}

	}
}
void UStatusComponent::StatusOnOff()
{
	if (!PlayerStatusWidget->IsInViewport())
	{
		PlayerStatusWidget->AddToViewport();
		//OwnerPlayerController->SetInputMode(FInputModeGameAndUI());
		//OwnerPlayerController->bShowMouseCursor = true;
	}
	else
	{
		PlayerStatusWidget->RemoveFromParent();
		//OwnerPlayerController->SetInputMode(FInputModeGameOnly());
		//OwnerPlayerController->bShowMouseCursor = false;
	}
}


void UStatusComponent::ApplyOption(float number, EOptionType optionType)
{
	switch (optionType)
	{
	case EOptionType::MaxHP:
		break;
	case EOptionType::DF:
		break;
	case EOptionType::Dmg:
		break;
	case EOptionType::Critical:
		break;
	default:
		break;
	}
}

