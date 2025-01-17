// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/ShotGunActorComponent.h"

#include <Interfaces/StatusInterface.h>
#include "InputTriggers.h"
#include "PlayerStatusComponent.h"
#include "CameraShake/ShotgunCameraShake.h"
#include "UI/HUD/ShotgunUI.h"

// Sets default values for this component's properties
UShotgunActorComponent::UShotgunActorComponent()
{
	WeaponType = EWeaponType::Shotgun;
}


// Called when the game starts
void UShotgunActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// 초기화
	//InitActorComponent();
}


// Called every frame
void UShotgunActorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UShotgunActorComponent::InitActorComponent()
{
	Super::InitActorComponent();

	// ShotGun Struct 데이터 세팅
	SetDefaultGunInfoStruct(TEXT("Shotgun"));

	// Crosshair UI 초기화
	if(GetWorld() && ShotgunUIClass)
	{
		ShotgunUI = CreateWidget<UShotgunUI>(GetWorld(), ShotgunUIClass);
		ShotgunUI->AddToViewport();
		ShotgunUI->SetVisibility(ESlateVisibility::Hidden);
		this->SetActive(false);

		this->CrosshairWidget = ShotgunUI;
	}
	
}

void UShotgunActorComponent::Fire(FVector StartLoc, FVector ForwardVector)
{
	for(int i=0; i < BulletNum; i++)
	{
		// 끝 위치 = 시작 위치에다가 (전방방향 * 총의 사격범위)를 더함
		FVector EndLoc = StartLoc + ForwardVector * GunInfoStruct.Range;
		
		FHitResult _hitRes;

		FCollisionQueryParams _collisionParam;
		_collisionParam.AddIgnoredActor(GetOwner());

		// Y, Z 방향의 기본 반동
		float DefaultSpreadY = Super::GetSpreadYRange();
		float DefaultSpreadZ = Super::GetSpreadZRange();

		//Y, Z 방향의 반동
		EndLoc.Y += FMath::RandRange(DefaultSpreadY * -1, DefaultSpreadY);
		EndLoc.Z += FMath::RandRange(DefaultSpreadZ * -1, DefaultSpreadZ);
		
		bool isHit = GetWorld()->LineTraceSingleByChannel(_hitRes, StartLoc, EndLoc, ECC_GameTraceChannel4, _collisionParam);
		DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Green, false, 3.f);
		
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
	}

	// 총알 감소
	GunInfoStruct.Magazine--;

	// 카메라 반동
	CameraShakeRecoil();

	Super::Fire(StartLoc, ForwardVector);
}

void UShotgunActorComponent::Reload()
{
	Super::Reload();
}

void UShotgunActorComponent::CameraShakeRecoil()
{
	Super::CameraShakeRecoil();

	//카메라 반동
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(UShotgunCameraShake::StaticClass());
}

TArray<struct FInputParameter> UShotgunActorComponent::ReturnInputParameter()
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

void UShotgunActorComponent::Input_Fire(const FInputActionValue& Value)
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
	PlayOwnerMontage(ShotgunFireMontage, 1.f);
}

void UShotgunActorComponent::Input_Reload()
{
	if(OnIsRemainAmmo.IsBound())
	{
		// 장전 가능 여부가 True면 장전 시퀀스 시작
		if(OnIsRemainAmmo.Execute(GunInfoStruct.MaxMagazine))
		{
			SetIsShootAble(false);
			GunInfoStruct.Magazine = 0;
			
			// 장전 애님몽타주 재생
			PlayOwnerMontage(ShotgunReloadMontage, RateReloadMontage);
		}
	}
}

void UShotgunActorComponent::Start_ZoomIn()
{
	Super::Start_ZoomIn();

	// Aiming 변수 변경
	SetIsAiming(true);
	
	// 스프링암과 UI
	if(ShotgunUI && ShotgunUI->IsVisible())
	{
		ShotgunUI->ZoomInCrosshair();
	}
	
}

void UShotgunActorComponent::Start_ZoomOut()
{
	Super::Start_ZoomOut();
	
	// Aiming 변수 변경
	SetIsAiming(false);

	// 스프링암과 UI
	if(ShotgunUI && ShotgunUI->IsVisible())
	{
		ShotgunUI->ZoomOutCrosshair();
	}
	
}

// 강화 옵션
void UShotgunActorComponent::SetShootInterval()
{
	if(GunSkillTree)
	{
		ShootIntervalOptionCnt++;
		TArray<FName> SkillTreeNamesArray = GunSkillTree->GetRowNames();
		FShotgunSkillParameter* DT_ShotgunData = GunSkillTree->FindRow<FShotgunSkillParameter>(SkillTreeNamesArray[ShootIntervalOptionCnt], TEXT(""));
		
		GunInfoStruct.ShootInterval = DT_ShotgunData->SetShootInterval;
	}
}

void UShotgunActorComponent::IncreaseBulletNum(/*int32 ShotgunBullet*/)
{
	if(GunSkillTree)
	{
		BulletNumOptionCnt++;
		TArray<FName> SkillTreeNamesArray = GunSkillTree->GetRowNames();
		FShotgunSkillParameter* DT_ShotgunData = GunSkillTree->FindRow<FShotgunSkillParameter>(SkillTreeNamesArray[BulletNumOptionCnt], TEXT(""));
		
		BulletNum += DT_ShotgunData->IncreaseBulletNum;
	}
}

void UShotgunActorComponent::SetRateReloadAnimMontage()
{
	if(GunSkillTree)
	{
		ReloadAnimOptionCnt++;
		TArray<FName> SkillTreeNamesArray = GunSkillTree->GetRowNames();
		FShotgunSkillParameter* DT_ShotgunData = GunSkillTree->FindRow<FShotgunSkillParameter>(SkillTreeNamesArray[ReloadAnimOptionCnt], TEXT(""));
		
		this->RateReloadMontage = DT_ShotgunData->SetReloadAnimRate;
	}
}

