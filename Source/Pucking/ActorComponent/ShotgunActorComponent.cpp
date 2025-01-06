// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/ShotGunActorComponent.h"

#include <Interfaces/StatusInterface.h>
#include "InputTriggers.h"
#include "PlayerStatusComponent.h"
#include "Character/PuckingPlayerCha.h"

// Sets default values for this component's properties
UShotgunActorComponent::UShotgunActorComponent()
{
	WeaponType = EWeaponType::Shotgun;
}


// Called when the game starts
void UShotgunActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ShotGun Struct 데이터 세팅
	SetDefaultGunInfoStruct(TEXT("Shotgun"));
	
}


// Called every frame
void UShotgunActorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Black, FString::Printf(TEXT("Shotgun ActorComponent Magazine is %d"), GunInfoStruct.Magazine));
}

void UShotgunActorComponent::Fire(FVector StartLoc, FVector ForwardVector)
{
	if(PlayerWeaponType == WeaponType)
	{
		for(int i=0; i < BulletNum; i++)
		{
			// 끝 위치 = 시작 위치에다가 (전방방향 * 총의 사격범위)를 더함
			FVector EndLoc = StartLoc + ForwardVector * GunInfoStruct.Range;
			
			FHitResult _hitRes;

			FCollisionQueryParams _collisionParam;
			_collisionParam.AddIgnoredActor(GetOwner());

			//Y, Z 방향의 반동
			EndLoc.Y += FMath::RandRange(GunInfoStruct.SpreadY * -1, GunInfoStruct.SpreadY);
			EndLoc.Z += FMath::RandRange(GunInfoStruct.SpreadZ * -1, GunInfoStruct.SpreadZ);
			
			bool isHit = GetWorld()->LineTraceSingleByChannel(_hitRes, StartLoc, EndLoc, ECC_Pawn, _collisionParam);
			DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Green, true, 5.f);
			
			if(isHit)
			{
				if(AActor* hitActor = _hitRes.GetActor())
				{
					IStatusInterface* StatInterface = Cast<IStatusInterface>(GetOwner()->FindComponentByClass<UPlayerStatusComponent>());
					if(StatInterface)
					{
						StatInterface->DamageProcessing(hitActor);
					}
				}
			}
		}

		// TODO 추후 구조 정해지면 적절한 곳으로 옮겨야함
		GunInfoStruct.Magazine--;

		// TODO 매개변수로 흔들림 조절할 수 있게 변경 필요
		CameraShakeRecoil();

		Super::Fire(StartLoc, ForwardVector);
	}
}

void UShotgunActorComponent::Reload()
{
	if(PlayerWeaponType == WeaponType)
	{
		Super::Reload();
	}
}

void UShotgunActorComponent::SetSpreadRange(float Y, float Z)
{
	Super::SetSpreadRange(Y, Z);
}

void UShotgunActorComponent::CameraShakeRecoil()
{
	Super::CameraShakeRecoil();

	//카메라 반동
	//GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(URifleCameraShake::StaticClass());
}

TArray<struct FInputParameter> UShotgunActorComponent::ReturnInputParameter()
{
	// Rifle Input 함수
	if(GunInputMappingContext)
	{
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

void UShotgunActorComponent::Input_Fire(const FInputActionValue& Value)
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
		PlayOwnerMontage(ShotgunFireMontage);
	}
}

void UShotgunActorComponent::Input_Reload()
{
	if(PlayerWeaponType == WeaponType)
	{
		Super::Input_Reload();

		// 장전 애님몽타주 재생
		PlayOwnerMontage(ShotgunReloadMontage);
	}
}
