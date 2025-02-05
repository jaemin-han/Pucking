// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/BFGActorComponent.h"

#include "InputTriggers.h"
#include "AOE/ProjectileBase.h"
#include "Blueprint/UserWidget.h"
#include "UI/HUD/CrosshairUI.h"

UBFGActorComponent::UBFGActorComponent()
{
	WeaponType = EWeaponType::BFG;
}

void UBFGActorComponent::InitActorComponent()
{
	Super::InitActorComponent();
	Super::InitActorComponent();

	if(USkeletalMeshComponent* CharacterSkeletal = GetOwner()->GetComponentByClass<USkeletalMeshComponent>())
	{
		Equip(CharacterSkeletal, FName("MachineGunSocket"), FTransform(FVector::ZeroVector));	
	}

	// Rifle Struct 데이터 세팅
	SetDefaultGunInfoStruct(TEXT("BFG"));

	// Crosshair UI 초기화
	if(GetWorld() && CrosshairUIClass)
	{
		CrosshairUI = CreateWidget<UCrosshairUI>(GetWorld(), CrosshairUIClass);
		CrosshairUI->AddToViewport();
		CrosshairUI->SetVisibility(ESlateVisibility::Hidden);
		this->SetActive(false);

		this->CrosshairWidget = CrosshairUI;
	}
	BFGProjectileClass = BFGProjectiles_Level[Level_BFG];
}

void UBFGActorComponent::Fire(FVector StartLoc, FVector ForwardVector)
{
	Super::Fire(StartLoc, ForwardVector);
	FActorSpawnParameters SpawnParameters;
	FRotator FireRotator = ForwardVector.Rotation();
	GetWorld()->SpawnActor<AProjectileBase>(BFGProjectileClass, StartLoc, FireRotator, SpawnParameters);
}

TArray<struct FInputParameter> UBFGActorComponent::ReturnInputParameter()
{
	Super::ReturnInputParameter();
	// Rifle Input 함수
	if(GunInputMappingContext)
	{
		// 초기화
		InputParameters.Empty();
		
		// Fire
		if(FireInputAction)
		{
			FInputParameter FireInputParameter;
			
			FireInputParameter.TargetClass = this;
			FireInputParameter.TriggerEvent = ETriggerEvent::Started;
			FireInputParameter.InputMappingContext = GunInputMappingContext;
			FireInputParameter.InputAction = FireInputAction;
			FireInputParameter.CallbackFunc = FName("Input_Fire");

			InputParameters.Add(FireInputParameter);
		}

		// Reload
		if(ReloadInputAction)
		{
			FInputParameter ReloadInputParameter;
			
			ReloadInputParameter.TargetClass = this;
			ReloadInputParameter.TriggerEvent = ETriggerEvent::Started;
			ReloadInputParameter.InputMappingContext = GunInputMappingContext;
			ReloadInputParameter.InputAction = ReloadInputAction;
			ReloadInputParameter.CallbackFunc = FName("Input_Reload");

			InputParameters.Add(ReloadInputParameter);
		}
	}
	return InputParameters;
}

void UBFGActorComponent::Input_Fire(const FInputActionValue& Value)
{
	Super::Input_Fire(Value);

	// 사격 불가능 상태면 return;
	if(!bIsShootAble) return;
	
	// 사격 애님몽타주 재생
	PlayOwnerMontage(ECharacterMontage::BFGFire);
}

void UBFGActorComponent::Input_Reload()
{
	//Super::Input_Reload();
	
	PlayOwnerMontage(ECharacterMontage::BFGReload);
}

void UBFGActorComponent::LevelUp()
{
	if(Level_BFG + 1 <= BFGProjectiles_Level.Num() - 1)
	{
		Level_BFG = Level_BFG + 1;
		BFGProjectileClass = BFGProjectiles_Level[Level_BFG];
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable To Level UP"));
	}
}
