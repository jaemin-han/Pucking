// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/StatusComponent.h"

#include "ActorComponent/EquipComponent.h"

#include "Item/OptionDataAsset.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"
#include "UI/Status/PlayerStatusWidget.h"
#include "NiagaraFunctionLibrary.h"
#include "UObject/ConstructorHelpers.h"


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
	CurStaggerResistance = StaggerResistance;
	CurStaggerValue = StaggerValue;

	//실드 나이아가라. 메테리얼 쓸수도
	NiagaraComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("ShieldNiagara"));

	//static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSysAsset(TEXT("/Script/Niagara.NiagaraSystem'/Game/sA_PickupSet_1/Fx/NiagaraSystems/NS_Shield_2.NS_Shield_2'"));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> NiagaraSysAsset(TEXT(""));

	if (NiagaraSysAsset.Succeeded())
	{
		NiagaraSys = NiagaraSysAsset.Object;
		NiagaraComp->SetAsset(NiagaraSys);
		NiagaraComp->bAutoActivate = false;
	}
	
	// ...
}


// Called when the game starts
void UStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	Owner = Cast<ACharacter>(GetOwner());
	OwnerPlayerController = Cast<APlayerController>(Owner->GetController());
	
	RemainHP = CurMaxHP;
	RemainShield = CurMaxShield;
	if (RemainShield <= 0)
	{
		NiagaraComp->SetActive(false, false);
	}
	else if (RemainShield > 0)
	{
		NiagaraComp->SetActive(true, false);
	}
	SetEnhancedInput();


	//디버그용
	//EnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EDamageType"), true);
	//if (!EnumPtr) return;
	

	// ...
	
}


// Called every frame
void UStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//캐릭터 위치 찾기
	//NiagaraComp->SetWorldLocation(GetOwner()->GetActorLocation());
	//NiagaraComp->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
	
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

void UStatusComponent::ApplyOptionByDataAssets(const TArray<UOptionDataAsset*>& OptionDataAssetArray)
{
	ResetStaticStatus();
	for (int32 i = 0; i < OptionDataAssetArray.Num(); i++)
	{
		auto* OptionDataAsset = OptionDataAssetArray[i];
		EOptionType GetOptionType = OptionDataAsset->GetOptionType();
		float GetOptionValue = OptionDataAsset->GetOptionValue();
		IncreaseOption(GetOptionType, GetOptionValue);
	}
}

void UStatusComponent::ShieldRecovery()
{
	if (RemainShield <= 0)
	{

		NiagaraComp->Deactivate();
		NiagaraComp->SetVisibility(false);

		RemainShield = 0;
	}
	//0보다 많으면 이펙트 켜기
	else if (RemainShield > 0)
	{
		NiagaraComp->SetVisibility(true);
		NiagaraComp->SetActive(false, true);
	}


	//현재 실드가 최대실드량보다 적으면
	if (RemainShield < CurMaxShield)
	{
		RemainShield++;
		//N초마다 이 ShieldRecovery함수 실행
		GetOwner()->GetWorld()->GetTimerManager().SetTimer(RecoverySpeedTimer, this, &UStatusComponent::ShieldRecovery, 0.001f, false);
	}
	//현재 실드가 최대실드량보다 같거나 커지면
	else if (RemainShield >= CurMaxShield)
	{
		//타이머 중단하고 함수 종료
		GetOwner()->GetWorld()->GetTimerManager().ClearTimer(RecoverySpeedTimer);
		return;
	}
}


void UStatusComponent::ApplyOption(EWeaponType WeaponType, int32 AmmoIndex)
{
	ResetStaticStatus();
	
	GetDataAssetArray = EquipComp->GetItemOptions(WeaponType, AmmoIndex);
	UE_LOG(LogTemp, Warning, TEXT("WeaponType : %s, AmmoIndex : %d"), *UEnum::GetValueAsString(WeaponType), AmmoIndex);
	for (int32 i = 0; i < GetDataAssetArray.Num(); i++)
	{
		auto* OptionDataAsset = GetDataAssetArray[i];
		EOptionType GetOptionType = OptionDataAsset->GetOptionType();
		float GetOptionValue = OptionDataAsset->GetOptionValue();
		IncreaseOption(GetOptionType, GetOptionValue);
	}

	//디버그용
	//EnumValueName = EnumPtr->GetNameStringByValue((int64)CommonDamageType);
}

void UStatusComponent::IncreaseOption(EOptionType OptionType, float OptionValue)
{
	switch (OptionType)
	{
	case EOptionType::DamageType:
		if (OptionValue == 0)
		{
			CommonDamageType = EDamageType::Physical;
		}
		else if (OptionValue == 1)
		{
			CommonDamageType = EDamageType::Fire;
		}
		else if (OptionValue == 2)
		{
			CommonDamageType = EDamageType::Ice;
		}
		break;
	case EOptionType::Damage:
		CurDamage += OptionValue;
		break;
	case EOptionType::CriticalRate:
		CurCriticalChance += OptionValue;
		break;
	case EOptionType::CriticalMultiplier:
		CurCriticalMultipier += OptionValue/100.0f;
		break;
	case EOptionType::PhysicalPenetration:
		CurPhysicalPenetration += OptionValue;
		break;
	case EOptionType::FirePenetration:
		CurFirePenetration += OptionValue;
		break;
	case EOptionType::IcePenetration:
		CurIcePenetration += OptionValue;
		break;
	case EOptionType::StaggerValue:
		CurStaggerValue += OptionValue;
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
	CurStaggerResistance = StaggerResistance;
	CurStaggerValue = StaggerValue;
	CommonDamageType = EDamageType::Physical;
}

