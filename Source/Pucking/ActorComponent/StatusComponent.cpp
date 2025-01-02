// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/StatusComponent.h"

#include "ActorComponent/EquipComponent.h"

#include "Item/OptionDataAsset.h"
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
	CurMaxHP = MaxHP;
	CurPhysicalDefense = PhysicalDefense;
	CurFireDefense = FireDefense;
	CurIceDefense = IceDefense;
	CurMaxShield = MaxShield;
	CurDamage = Damage;
	CurCriticalChance = CriticalChance;
	CurCriticalMultipier = CriticalMultipier;
	CurPhysicalPenetration = PhysicalPenetration;
	CurFirePenetration = FirePenetration;
	CurIcePenetration = IcePenetration;
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

	EquipComp->OnStatusComponentChanged.AddDynamic(this, &UStatusComponent::ApplyOption);
	// ...
	
}


// Called every frame
void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	GEngine->AddOnScreenDebugMessage(-1, 0.005f, FColor::Green, FString::Printf(TEXT("MaxHP : %f"), CurMaxHP));
	GEngine->AddOnScreenDebugMessage(-1, 0.005f, FColor::Green, FString::Printf(TEXT("Defense : %f"), CurPhysicalDefense));
	GEngine->AddOnScreenDebugMessage(-1, 0.005f, FColor::Green, FString::Printf(TEXT("Damage : %f"), CurDamage));
	GEngine->AddOnScreenDebugMessage(-1, 0.005f, FColor::Green, FString::Printf(TEXT("CriticalChance : %f"), CurCriticalChance));
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


void UStatusComponent::ApplyOption(EWeaponType WeaponType, int32 AmmoIndex)
{
	ResetStaticStatus();
	
	GetDataAssetArray = EquipComp->GetItemOptions(WeaponType, AmmoIndex);
	for (int32 i = 0; i < GetDataAssetArray.Num(); i++)
	{
		auto* OptionDataAsset = GetDataAssetArray[i];
		EOptionType GetOptionType = OptionDataAsset->GetOptionType();
		float GetOptionValue = OptionDataAsset->GetOptionValue();
		IncreaseOption(GetOptionType, GetOptionValue);
	}
	/*CurrentDataAssetArray = EquipComp->GetItemOptions(CurrentWeaponType, CurrentAmmoIndex);
	for (int32 j = 0; j < CurrentDataAssetArray.Num(); j++)
	{
		auto* CurOptionDataAsset = CurrentDataAssetArray[j];
		EOptionType CurGetOptionType = CurOptionDataAsset->GetOptionType();
		float CurGetOptionValue = CurOptionDataAsset->GetOptionValue();
		DecreaseOption(CurGetOptionType, CurGetOptionValue);
	}
	CurrentWeaponType = WeaponType;
	CurrentAmmoIndex = AmmoIndex;*/
}

void UStatusComponent::IncreaseOption(EOptionType OptionType, float OptionValue)
{
	switch (OptionType)
	{
	case EOptionType::MaxHP:
		//if (OptionValue)
		{
			CurMaxHP += OptionValue;
			
		}
		break;
	case EOptionType::DF:
		//if (OptionValue)
		{
			CurPhysicalDefense += OptionValue;
			
		}
		break;
	case EOptionType::Dmg:
		//if (OptionValue)
		{
			CurDamage += OptionValue;
			
		}
		break;
	case EOptionType::Critical:
		//if (OptionValue)
		{
			CurCriticalChance += OptionValue;
			
		}
		break;
	default:
		break;
	}
}

void UStatusComponent::ResetStaticStatus()
{
	CurMaxHP = MaxHP;
	CurPhysicalDefense = PhysicalDefense;
	CurFireDefense = FireDefense;
	CurIceDefense = IceDefense;
	CurMaxShield = MaxShield;
	CurDamage = Damage;
	CurCriticalChance = CriticalChance;
	CurCriticalMultipier = CriticalMultipier;
	CurPhysicalPenetration = PhysicalPenetration;
	CurFirePenetration = FirePenetration;
	CurIcePenetration = IcePenetration;
}

