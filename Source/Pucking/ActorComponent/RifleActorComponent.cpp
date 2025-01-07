// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/RifleActorComponent.h"

#include <Interfaces/StatusInterface.h>

#include "InputTriggers.h"
#include "PlayerStatusComponent.h"
#include "CameraShake/RifleCameraShake.h"
#include "Character/PuckingPlayerCha.h"
#include "UI/HUD/ReticleUI.h"

URifleActorComponent::URifleActorComponent()
{
	WeaponType = EWeaponType::Rifle;
}

void URifleActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// Rifle Struct 데이터 세팅
	SetDefaultGunInfoStruct(TEXT("Rifle"));

	//TODO 수정 필요
	if(GetOwner()) PlayerCha = Cast<APuckingPlayerCha>(GetOwner());
}

void URifleActorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(GetOwner())
	{
		bool IsCharacterAction = GetOwner()->GetVelocity().Normalize();
		if(!IsExtendSpread && !IsCharacterAction)
		{
			if(MultiplySpreadPerSec > 0.f)
			{
				MultiplySpreadPerSec -= DeltaTime;

				//TODO 수정 필요
				if(PlayerCha && PlayerCha->ReticleUI) PlayerCha->ReticleUI->SetReticlePosition(MultiplySpreadPerSec * -1);
			}
		}
		else
		{
			if(MultiplySpreadPerSec < MaxSpread)
			{
				MultiplySpreadPerSec += DeltaTime;
				
				//TODO 수정 필요
				if(PlayerCha && PlayerCha->ReticleUI) PlayerCha->ReticleUI->SetReticlePosition(MultiplySpreadPerSec);
			}
		}
	}
}

void URifleActorComponent::Fire(FVector StartLoc, FVector ForwardVector)
{
	IsExtendSpread = true;
	
	if(PlayerWeaponType == WeaponType)
	{
		// 끝 위치 = 시작 위치에다가 (전방방향 * 총의 사격범위)를 더함
		FVector EndLoc = StartLoc + ForwardVector * GunInfoStruct.Range;
		
		FHitResult _hitRes;

		FCollisionQueryParams _collisionParam;
		_collisionParam.AddIgnoredActor(GetOwner());

		//Y, Z 방향의 반동
		EndLoc.Y += FMath::RandRange((GunInfoStruct.SpreadY * MultiplySpreadPerSec) * -1, (GunInfoStruct.SpreadY * MultiplySpreadPerSec));
		EndLoc.Z += FMath::RandRange((GunInfoStruct.SpreadZ * MultiplySpreadPerSec) * -1, (GunInfoStruct.SpreadZ * MultiplySpreadPerSec));
		
		bool isHit = GetWorld()->LineTraceSingleByChannel(_hitRes, StartLoc, EndLoc, ECC_Pawn, _collisionParam);
		DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Green, true, 5.f);
		
		if(isHit)
		{
			if(AActor* hitActor = _hitRes.GetActor())
			{
				// 다른 StatusActorComponent 함수 직접 호출
				IStatusInterface* StatInterface = Cast<IStatusInterface>(GetOwner()->FindComponentByClass<UPlayerStatusComponent>());
				if(StatInterface)
				{
					StatInterface->DamageProcessing(hitActor, _hitRes);
				}
			}
		}

		GunInfoStruct.Magazine--;
		
		CameraShakeRecoil();

		Super::Fire(StartLoc, ForwardVector);

		GetWorld()->GetTimerManager().ClearTimer(SpreadTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(SpreadTimerHandle, [this]()
		{
			IsExtendSpread = false;
		}, 2.f, false);
	}
}

void URifleActorComponent::Reload()
{
	if(PlayerWeaponType == WeaponType)
	{
		Super::Reload();
	}
}

void URifleActorComponent::SetSpreadRange(float Y, float Z)
{
	Super::SetSpreadRange(Y, Z);
}

void URifleActorComponent::CameraShakeRecoil()
{
	Super::CameraShakeRecoil();

	//카메라 반동
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(URifleCameraShake::StaticClass());
}

TArray<struct FInputParameter> URifleActorComponent::ReturnInputParameter()
{
	// Rifle Input 함수
	if(GunInputMappingContext)
	{
		// Fire
		if(FireInputAction)
		{
			FInputParameter FireInputParameter;
			
			FireInputParameter.TargetClass = this;
			FireInputParameter.TriggerEvent = ETriggerEvent::Triggered;
			FireInputParameter.InputMappingContext = GunInputMappingContext;
			FireInputParameter.InputAction = FireInputAction;
			FireInputParameter.CallbackFunc = FName("Input_Fire");

			InputParameters.Add(FireInputParameter);
		}

		// Reload
		if(ReloadInputAction)
		{
			// Reload Input 함수
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

void URifleActorComponent::Input_Fire(const FInputActionValue& Value)
{
	if(PlayerWeaponType == WeaponType)
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
		PlayOwnerMontage(RifleFireMontage);
	}
}

void URifleActorComponent::Input_Reload()
{
	if(PlayerWeaponType == WeaponType)
	{
		Super::Input_Reload();

		// 장전 애님몽타주 재생
		PlayOwnerMontage(RifleReloadMontage);
	}
}
