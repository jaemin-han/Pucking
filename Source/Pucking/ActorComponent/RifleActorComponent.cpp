// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/RifleActorComponent.h"

#include <Interfaces/StatusInterface.h>

#include "InputTriggers.h"
#include "PlayerStatusComponent.h"
#include "CameraShake/RifleCameraShake.h"
#include "UI/HUD/CrosshairUI.h"


URifleActorComponent::URifleActorComponent()
{
	WeaponType = EWeaponType::Rifle;
}

void URifleActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// 초기화
	//InitActorComponent();
}

void URifleActorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 사격을 안 하고 있으면 보정값을 원래대로
	if(!IsExtendSpread && FireExtendSpread > 0.f)
	{
		FireExtendSpread -= DeltaTime * 100.f;

		if(FMath::IsNearlyEqual(0.f, FireExtendSpread, 0.1f))
		{
			FireExtendSpread = 0.f;
		}
	}

	// 이동 속도에 따른 Crosshair UI 벌어짐 정도 범위 설정
	if(GetOwner())
	{
		// Range값들이 초기화 되어있는지 확인
		if(InputSpreadRange.HasUpperBound() && OutputSpreadRange.HasUpperBound())
		{
			// 캐릭터 속도 
			float OwnerVectorLength = GetOwner()->GetVelocity().Length();

			// 캐릭터 속도를 UI 벌어짐 보정값에 맞게 변환
			float ChangeVal = FMath::GetMappedRangeValueClamped(InputSpreadRange, OutputSpreadRange, OwnerVectorLength);

			// UI에 반영
			// 사격할 때의 집탄율 마이너스 보정값 추가
			CrosshairUI->SetCrosshairPosition(ChangeVal + FireExtendSpread);

			// UI에 반영된 벌어진 정도를 비율로 계산
			float ConvertRange = FMath::GetMappedRangeValueClamped(InputSpreadRange, TRange<float>(1.f, 2.f), OwnerVectorLength);
			MultiplySpread = ConvertRange;
		}
	}
}

void URifleActorComponent::InitActorComponent()
{
	Super::InitActorComponent();

	// Rifle Struct 데이터 세팅
	SetDefaultGunInfoStruct(TEXT("Rifle"));

	// Crosshair UI 초기화
	if(GetWorld() && CrosshairUIClass)
	{
		CrosshairUI = CreateWidget<UCrosshairUI>(GetWorld(), CrosshairUIClass);
		CrosshairUI->AddToViewport();
		CrosshairUI->SetVisibility(ESlateVisibility::Hidden);
		this->SetActive(false);

		this->CrosshairWidget = CrosshairUI;
	}

	// Crosshair UI 벌어진 정도에 반영될 캐릭터 최대 속도
	InputSpreadRange = TRange<float>(0.f, GunInfoStruct.PlayerMaxSpd);
	
	// Crosshair UI 벌어진 정도를 보정할 값 설정   
	OutputSpreadRange = TRange<float>(0.f, GunInfoStruct.MaxUISpreadPerSpd);
}

void URifleActorComponent::Fire(FVector StartLoc, FVector ForwardVector)
{
	IsExtendSpread = true;
	
	//if(PlayerWeaponType == WeaponType)
	{
		// 끝 위치 = 시작 위치에다가 (전방방향 * 총의 사격범위)를 더함
		FVector EndLoc = StartLoc + ForwardVector * GunInfoStruct.Range;
		
		FHitResult _hitRes;

		FCollisionQueryParams _collisionParam;
		_collisionParam.AddIgnoredActor(GetOwner());

		// Y, Z 방향의 기본 반동
		float DefaultSpreadY = Super::GetSpreadYRange();
		float DefaultSpreadZ = Super::GetSpreadZRange();
		
		// 기본 반동 * UI가 벌어진만큼 비율 + 사격에 따른 보정값
		EndLoc.Y += FMath::RandRange(((DefaultSpreadY * MultiplySpread) + FireExtendSpread) * -1, ((DefaultSpreadY * MultiplySpread + FireExtendSpread)));
		EndLoc.Z += FMath::RandRange(((DefaultSpreadZ * MultiplySpread) + FireExtendSpread) * -1, ((DefaultSpreadZ * MultiplySpread + FireExtendSpread)));
		
		bool isHit = GetWorld()->LineTraceSingleByChannel(_hitRes, StartLoc, EndLoc, ECC_GameTraceChannel2, _collisionParam);
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

		// 총알 감소
		GunInfoStruct.Magazine--;

		// 카메라 반동
		CameraShakeRecoil();

		// 공통적인 기능 - 머즐 이펙트, 사운드 
		Super::Fire(StartLoc, ForwardVector);

		// 1초 동안 사격 안 하면 보정값 원래대로
		GetWorld()->GetTimerManager().ClearTimer(SpreadTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(SpreadTimerHandle, [this]()
		{
			IsExtendSpread = false;
		}, 1.f, false);
		
	}
}

void URifleActorComponent::Reload()
{
	//if(PlayerWeaponType == WeaponType)
	{
		Super::Reload();
	}
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
		// 초기화
		InputParameters.Empty();
		
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

		// Zoom
		if(ZoomAction)
		{
			// Zoom In Input 함수
			FInputParameter ZoomInInputParameter;
	
			ZoomInInputParameter.TargetClass = this;
			ZoomInInputParameter.TriggerEvent = ETriggerEvent::Started;
			ZoomInInputParameter.InputMappingContext = GunInputMappingContext;
			ZoomInInputParameter.InputAction = ZoomAction;
			ZoomInInputParameter.CallbackFunc = FName("Start_ZoomIn");

			InputParameters.Add(ZoomInInputParameter);
			
			// Zoom Out Input 함수
			FInputParameter ZoomOutInputParameter;
	
			ZoomOutInputParameter.TargetClass = this;
			ZoomOutInputParameter.TriggerEvent = ETriggerEvent::Completed;
			ZoomOutInputParameter.InputMappingContext = GunInputMappingContext;
			ZoomOutInputParameter.InputAction = ZoomAction;
			ZoomOutInputParameter.CallbackFunc = FName("Start_ZoomOut");

			InputParameters.Add(ZoomOutInputParameter);
		}
		
	}
	
	return InputParameters;
}

void URifleActorComponent::Input_Fire(const FInputActionValue& Value)
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
	
	// 사격 시 집탄율 마이너스 보정값 증가
	if(FireExtendSpread < GunInfoStruct.MaxUISpreadPerFire)
	{
		if(GetIsAiming())
		{
			FireExtendSpread += 2.f;
		}
		else
		{
			FireExtendSpread += 10.f;	
		}
	}
	
	// 사격 애님몽타주 재생
	PlayOwnerMontage(RifleFireMontage, 1.f);
}

void URifleActorComponent::Input_Reload()
{
	//if(PlayerWeaponType == WeaponType)
	{
		if(OnIsRemainAmmo.IsBound())
		{
			// 장전 가능 여부가 True면 장전 시퀀스 시작
			if(OnIsRemainAmmo.Execute(GunInfoStruct.MaxMagazine))
			{
				SetIsShootAble(false);
				GunInfoStruct.Magazine = 0;
				
				// 장전 애님몽타주 재생
				PlayOwnerMontage(RifleReloadMontage, RateReloadMontage);
			}
		}
	}
	
}

void URifleActorComponent::Start_ZoomIn()
{
	Super::Start_ZoomIn();

	// Aiming 변수 변경
	SetIsAiming(true);
	
	// 스프링암과 UI
	if(CrosshairUI && CrosshairUI->IsVisible())
	{
		CrosshairUI->ZoomInCrosshair();
	}
	
}

void URifleActorComponent::Start_ZoomOut()
{
	Super::Start_ZoomOut();

	// Aiming 변수 변경
	SetIsAiming(false);

	// 스프링암과 UI
	if(CrosshairUI && CrosshairUI->IsVisible())
	{
		CrosshairUI->ZoomOutCrosshair();
	}
	
}

// 강화 옵션
void URifleActorComponent::DecreaseSpreadRange()
{
	if(GunSkillTree)
	{
		SpreadOptionCnt++;
		TArray<FName> SkillTreeNamesArray = GunSkillTree->GetRowNames();
		FRifleSkillParameter* DT_RifleData = GunSkillTree->FindRow<FRifleSkillParameter>(SkillTreeNamesArray[SpreadOptionCnt], TEXT(""));
		
		GunInfoStruct.SpreadY = DT_RifleData->DecreaseSpreadY;
		GunInfoStruct.SpreadZ = DT_RifleData->DecreaseSpreadZ;
	}
}

void URifleActorComponent::IncreaseMaxMagazine()
{
	if(GunSkillTree)
	{
		MaxMagazineOptionCnt++;
		TArray<FName> SkillTreeNamesArray = GunSkillTree->GetRowNames();
		FRifleSkillParameter* DT_RifleData = GunSkillTree->FindRow<FRifleSkillParameter>(SkillTreeNamesArray[MaxMagazineOptionCnt], TEXT(""));
		
		GunInfoStruct.MaxMagazine = DT_RifleData->IncreaseMaxMagazine;
	}
}

void URifleActorComponent::SetRateReloadAnimMontage()
{
	if(GunSkillTree)
	{
		ReloadAnimOptionCnt++;
		TArray<FName> SkillTreeNamesArray = GunSkillTree->GetRowNames();
		FRifleSkillParameter* DT_RifleData = GunSkillTree->FindRow<FRifleSkillParameter>(SkillTreeNamesArray[ReloadAnimOptionCnt], TEXT(""));

		this->RateReloadMontage = DT_RifleData->SetReloadAnimRate;
	}
}
