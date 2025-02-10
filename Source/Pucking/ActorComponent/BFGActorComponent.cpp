// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/BFGActorComponent.h"

#include "InputTriggers.h"
#include "AOE/ProjectileBase.h"
#include "Blueprint/UserWidget.h"
#include "CameraShake/BFGFirstCameraShake.h"
#include "CameraShake/BFGSecondCameraShake.h"
#include "CameraShake/BFGThirdCameraShake.h"
#include "UI/HUD/ShotgunUI.h"

UBFGActorComponent::UBFGActorComponent()
{
	WeaponType = EWeaponType::BFG;
}

void UBFGActorComponent::InitActorComponent()
{
	Super::InitActorComponent();

	if(USkeletalMeshComponent* CharacterSkeletal = GetOwner()->GetComponentByClass<USkeletalMeshComponent>())
	{
		Equip(CharacterSkeletal, FName("MachineGunSocket"), FTransform(FVector::ZeroVector));	
	}

	// Rifle Struct 데이터 세팅
	SetDefaultGunInfoStruct(TEXT("BFG"));
	
	BFGProjectileClass = BFGProjectiles_Level[Level_BFG];
}

void UBFGActorComponent::Fire(FVector StartLoc, FVector ForwardVector)
{
	// 총알 감소
	GunInfoStruct.Magazine--;
	
	Super::Fire(StartLoc, ForwardVector);
	FActorSpawnParameters SpawnParameters;
	FRotator FireRotator = ForwardVector.Rotation();
	GetWorld()->SpawnActor<AProjectileBase>(BFGProjectileClass, StartLoc, FireRotator, SpawnParameters);

}

void UBFGActorComponent::Reload()
{
	// 총알 관련 Delegate에 바운드 되어있는지 확인
	if(OnRemainAmmo.IsBound())
	{
		// 총 데미지 타입 Delegate에 바운드 확인
		if (OnGetDamageType.IsBound())
		{
			DamageType = OnGetDamageType.Execute();
		}
		int32 RemainAmmo = OnRemainAmmo.Execute(GunInfoStruct.MaxMagazine);
		GunInfoStruct.Magazine += RemainAmmo;
		SetIsShootAble(true);

		if(OnReloadDelegate.IsBound())
		{
			OnReloadDelegate.Broadcast(GunInfoStruct.MaxMagazine);
			if(OnFireDelegate.IsBound())
			{
				OnFireDelegate.Broadcast(GunInfoStruct.Magazine);
			}
		}
	}
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

	// 남은 총알 확인
	if(GunInfoStruct.Magazine <= 0)
	{
		Input_Reload();
		return;
	}
	
	// 사격 애님몽타주 재생
	if(GetIsAiming())
	{
		PlayOwnerMontage(ECharacterMontage::BFGZoomFire);
	}
	else
	{
		PlayOwnerMontage(ECharacterMontage::BFGFire);
	}

	//CameraShakeRecoil();
}

void UBFGActorComponent::Input_Reload()
{
	//Super::Input_Reload();

	if(!IsCanPlayMontageState(ECharacterMontage::BFGReload)) return;
	
	if(OnIsRemainAmmo.IsBound())
	{
		// 장전 가능 여부가 True면 장전 시퀀스 시작
		if(OnIsRemainAmmo.Execute(GunInfoStruct.MaxMagazine))
		{
			SetIsShootAble(false);
			GunInfoStruct.Magazine = 0;

			if(OnFireDelegate.IsBound())
			{
				OnFireDelegate.Broadcast(0);
			}

			if(OnReloadDelegate.IsBound())
			{
				OnReloadDelegate.Broadcast(GunInfoStruct.MaxMagazine);
			}
			
			// 장전 애님몽타주 재생
			PlayOwnerMontage(ECharacterMontage::BFGReload);
		}
	}
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

void UBFGActorComponent::CameraShakeRecoil()
{
	Super::CameraShakeRecoil();

	switch (Level_BFG)
	{
		case 0:
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(UBFGFirstCameraShake::StaticClass());
			break;
		case 1:
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(UBFGSecondCameraShake::StaticClass());
			break;
		case 2:
			GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(UBFGThirdCameraShake::StaticClass());
			break;
		default:
			break;
	}
}