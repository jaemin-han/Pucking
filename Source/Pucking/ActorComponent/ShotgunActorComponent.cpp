// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/ShotGunActorComponent.h"

#include <Interfaces/StatusInterface.h>
#include "InputTriggers.h"
#include "PlayerStatusComponent.h"
#include "CameraShake/ShotgunCameraShake.h"
#include "Character/PuckAnimInstance.h"
#include "GameFramework/Character.h"
#include "Item/BulletProjectile.h"
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

	if(USkeletalMeshComponent* CharacterSkeletal = GetOwner()->GetComponentByClass<USkeletalMeshComponent>())
	{
		Equip(CharacterSkeletal, ShotgunEquipSocket, ShotgunEquipTransform);
		if(GunBlueprintClass)
		{
			BP_GunActor = GetWorld()->SpawnActor<AActor>(GunBlueprintClass);
			BP_GunActor->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, ShotgunEquipSocket);
			BP_GunActor->SetActorRelativeTransform(ShotgunEquipTransform);
			BP_GunActor->RegisterAllComponents();

			// 처음에는 Hidden
			BP_GunActor->SetHidden(true);
		}
	}

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

	/*if(OwnerCharacter && OwnerCharacter->GetMesh() &&OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		UPuckAnimInstance* AnimIns = Cast<UPuckAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
		if(AnimIns)
		{
			AnimIns->OnChangeFsm.AddDynamic(this, &UShotgunActorComponent::GetCurrentFsm);
		}
	}*/
}

void UShotgunActorComponent::Fire(FVector StartLoc, FVector ForwardVector)
{
	// 카메라 반동
	CameraShakeRecoil();
	Super::Fire(StartLoc, ForwardVector);
	
	for(int i=0; i < BulletNum; i++)
	{
		// 끝 위치 = 시작 위치에다가 (전방방향 * 총의 사격범위)를 더함
		FVector EndLoc = StartLoc + ForwardVector * GunInfoStruct.Range;
		
		FHitResult _hitRes;

		FCollisionQueryParams _collisionParam;
		_collisionParam.AddIgnoredActor(GetOwner());

		// Y, Z 방향의 기본 반동
		float DefaultSpreadX = Super::GetSpreadXRange();
		float DefaultSpreadY = Super::GetSpreadYRange();
		float DefaultSpreadZ = Super::GetSpreadZRange();
		
		//Y, Z 방향의 반동
		EndLoc.X += FMath::RandRange(DefaultSpreadX * -1, DefaultSpreadX);
		EndLoc.Y += FMath::RandRange(DefaultSpreadY * -1, DefaultSpreadY);
		EndLoc.Z += FMath::RandRange(DefaultSpreadZ * -1, DefaultSpreadZ);
		
		bool isHit = GetWorld()->LineTraceSingleByChannel(_hitRes, StartLoc, EndLoc, ECC_GameTraceChannel5, _collisionParam);
		//DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Green, false, 5.f);
		FVector FireVector = EndLoc - StartLoc;
		FActorSpawnParameters SpawnParameters;
		FRotator FireRotator = FireVector.Rotation();
		if(DamageType == EDamageType::Fire)
		{
			GetWorld()->SpawnActor<ABulletProjectile>(FireBulletProjectileClass, MuzzleLoc, FireRotator, SpawnParameters);
		}
		else if(DamageType == EDamageType::Ice)
		{

			GetWorld()->SpawnActor<ABulletProjectile>(IceBulletProjectileClass, MuzzleLoc, FireRotator, SpawnParameters);
		}
		else
		{
			GetWorld()->SpawnActor<ABulletProjectile>(NormalBulletProjectileClass, MuzzleLoc, FireRotator, SpawnParameters);
		}
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

void UShotgunActorComponent::GetCurrentFsm(ECharacterFSM TargetFsm)
{
	Super::GetCurrentFsm(TargetFsm);
	
}

void UShotgunActorComponent::Input_Fire(const FInputActionValue& Value)
{
	if(!IsCanPlayMontageState(ECharacterMontage::ShotgunFire)) return;
	
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
		PlayOwnerMontage(ECharacterMontage::ShotgunZoomFire, (1 / GunInfoStruct.ShootInterval));
	}
	else
	{
		PlayOwnerMontage(ECharacterMontage::ShotgunFire, (1 / GunInfoStruct.ShootInterval));
	}

	// 샷건 Actor Animation
	if(BP_GunActor->GetClass()->ImplementsInterface(UFireInterface::StaticClass()))
	{
		IFireInterface::Execute_FireUsedBP(BP_GunActor);
	}
}

void UShotgunActorComponent::Input_Reload()
{
	if(!IsCanPlayMontageState(ECharacterMontage::ShotgunReload)) return;
	
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
				OnReloadDelegate.Broadcast(0);
			}
			
			// 장전 애님몽타주 재생
			PlayOwnerMontage(ECharacterMontage::ShotgunReload, RateReloadMontage);

			// 샷건 Actor Animation
			if(BP_GunActor->GetClass()->ImplementsInterface(UReloadInterface::StaticClass()))
			{
				IReloadInterface::Execute_ReloadUsedBP(BP_GunActor, RateReloadMontage);
			}
		}
	}
}

void UShotgunActorComponent::Start_ZoomIn()
{
	Super::Start_ZoomIn();

	// 줌 가능한 상태인지 체크
	if(IsCanChangeState(ECharacterFSM::Zoom))
	{
		// 스프링암과 UI
		if(ShotgunUI && ShotgunUI->IsVisible())
		{
			ShotgunUI->ZoomInCrosshair();
			ChangeState(ECharacterFSM::Zoom);
		}
		
		// Aiming 변수 변경
		SetIsAiming(true);
	}
}

void UShotgunActorComponent::Start_ZoomOut()
{
	Super::Start_ZoomOut();

	// 스프링암과 UI
	if(ShotgunUI && ShotgunUI->IsVisible())
	{
		if(GetIsAiming())
		{
			ShotgunUI->ZoomOutCrosshair();
			ChangeState(ECharacterFSM::Idle);
		}
	}

	// Aiming 변수 변경
	SetIsAiming(false);
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

