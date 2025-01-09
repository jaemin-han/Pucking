// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/GunActorComponent.h"

#include "EquipComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/CrosshairUI.h"

// Sets default values for this component's properties
UGunActorComponent::UGunActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	bIsShootAble = true;
}


// Called when the game starts
void UGunActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// Owner Actor를 먼저 확인
	if(GetOwner())
	{
		if(USkeletalMeshComponent* CharacterSkeletal = GetOwner()->GetComponentByClass<USkeletalMeshComponent>())
		{
			Equip(CharacterSkeletal, FName("GunSocket"), FTransform(FVector::ZeroVector));	
		}

		// UEquipComponent Interface 가지고 있는지 확인
		UEquipComponent* EquipComponent = GetOwner()->FindComponentByClass<UEquipComponent>();
		if(EquipComponent)
		{
			EquipComponent->OnWeaponTypeChanged.AddDynamic(this, &UGunActorComponent::SetCurrentOwnerWeaponType);	
		}

		// Montage 재생할 때 필요한 Character로 캐싱
		if(Cast<ACharacter>(GetOwner()))
		{
			OwnerCharacter = Cast<ACharacter>(GetOwner());
		}
	}
	
}


// Called every frame
void UGunActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGunActorComponent::SetDefaultGunInfoStruct(FName TableRows)
{
	// DataTable 정보를 세팅
	if (GunInfoDataTable)
	{
		FGunInfoStruct* DT_GunData = GunInfoDataTable->FindRow<FGunInfoStruct>(FName(TableRows), TEXT(""));
		
		GunInfoStruct.GunType = DT_GunData->GunType;
		GunInfoStruct.Magazine = DT_GunData->Magazine;
		GunInfoStruct.MaxMagazine = DT_GunData->MaxMagazine;
		GunInfoStruct.DefaultDamage = DT_GunData->DefaultDamage;
		GunInfoStruct.SpreadY = DT_GunData->SpreadY;
		GunInfoStruct.SpreadZ = DT_GunData->SpreadZ;
		/*GunInfoStruct.RecoilPitch = DT_GunData->RecoilPitch;
		GunInfoStruct.RecoilYaw = DT_GunData->RecoilYaw;*/
		GunInfoStruct.Range = DT_GunData->Range;
		GunInfoStruct.ShootInterval = DT_GunData->ShootInterval;

		GunInfoStruct.PlayerMaxSpd = DT_GunData->PlayerMaxSpd;
		GunInfoStruct.MaxUISpreadPerSpd = DT_GunData->MaxUISpreadPerSpd;
		GunInfoStruct.MaxUISpreadPerFire = DT_GunData->MaxUISpreadPerFire;
		GunInfoStruct.ModifyZoomRecoil = DT_GunData->ModifyZoomRecoil;
	}
}

void UGunActorComponent::Equip(USkeletalMeshComponent* TargetSkeletalMeshComp, FName SocketName, FTransform ActorTransform)
{
	if(GunSkeletalMesh)
	{
		SkeletalMeshComponent = NewObject<USkeletalMeshComponent>(TargetSkeletalMeshComp->GetOwner());
		if(SkeletalMeshComponent)
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

			// todo: 재민 수정
			// SkeletalMeshComponent->SetRelativeRotation(FRotator(90, 0, 180));
			SkeletalMeshComponent->SetSkeletalMesh(GunSkeletalMesh);
			SkeletalMeshComponent->AttachToComponent(TargetSkeletalMeshComp, FAttachmentTransformRules::KeepRelativeTransform, SocketName);
			SkeletalMeshComponent->RegisterComponent();
		}
	}
}

void UGunActorComponent::Fire(FVector StartLoc, FVector ForwardVector)
{
	// 발사 직후 사격 불가능 상태
	this->SetIsShootAble(false);

	// 총 사격 딜레이
	FTimerHandle ShootAbleTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(ShootAbleTimerHandle, [this]()
	{
		this->SetIsShootAble(true);
	}, GunInfoStruct.ShootInterval, false);
	
	if(MuzzleParticle)
	{
		FVector MuzzleLoc = SkeletalMeshComponent->GetSocketLocation(FName("Muzzle"));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleParticle, MuzzleLoc, FRotator(0, 0, 0));	
	}
}

void UGunActorComponent::Reload()
{
	// Delegate에 바운드 되어있는지 확인
	if(OnRemainAmmo.IsBound())
	{
		int32 RemainAmmo = OnRemainAmmo.Execute(GunInfoStruct.MaxMagazine);
		GunInfoStruct.Magazine += RemainAmmo;
		SetIsShootAble(true);		
	}
}

void UGunActorComponent::SetShootInterval(float IntervalTime)
{
	GunInfoStruct.ShootInterval = IntervalTime;
}

bool UGunActorComponent::GetIsShootAble()
{
	return bIsShootAble;
}

void UGunActorComponent::SetIsShootAble(bool ShootAble)
{
	bIsShootAble = ShootAble;
}


void UGunActorComponent::SetCurrentOwnerWeaponType(EWeaponType ChangeWeaponType)
{
	// SkeletalMesh 체크
	if(!SkeletalMeshComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("No Weapon SkeletalMeshComponent"));
		return;
	}
	
	// 현재 플레이어의 무기 캐싱
	PlayerWeaponType = ChangeWeaponType;

	// 타입이 자신이면 Visible true
	if(WeaponType == ChangeWeaponType)
	{
		//Activate();
		SkeletalMeshComponent->SetVisibility(true);
		if(CrosshairWidget)
		{
			CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		//Deactivate();
		SkeletalMeshComponent->SetVisibility(false);
		if(CrosshairWidget)
		{
			CrosshairWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
}

void UGunActorComponent::SetIsAiming(bool CurrentAiming)
{
	bIsAiming = CurrentAiming;
}

bool UGunActorComponent::GetIsAiming() const
{
	return bIsAiming;
}

TArray<FInputParameter> UGunActorComponent::ReturnInputParameter()
{
	return InputParameters;
}

void UGunActorComponent::Input_Fire(const FInputActionValue& Value)
{
}

void UGunActorComponent::Input_Reload()
{
	if(OnIsRemainAmmo.IsBound())
	{
		// 장전 가능 여부가 True면 장전 시퀀스 시작
		if(OnIsRemainAmmo.Execute(GunInfoStruct.MaxMagazine))
		{
			SetIsShootAble(false);
			GunInfoStruct.Magazine = 0;
		}
	}
}

float UGunActorComponent::GetSpreadYRange()
{
	// DataTable에서 기본 반동값 가져옴
	float DefaultRecoilY = GunInfoStruct.SpreadY;

	// 조준 중이면 절반
	if(GetIsAiming())
	{
		DefaultRecoilY *= GunInfoStruct.ModifyZoomRecoil;
	}
	return DefaultRecoilY;
}

float UGunActorComponent::GetSpreadZRange()
{
	// DataTable에서 기본 반동값 가져옴
	float DefaultRecoilZ = GunInfoStruct.SpreadZ;

	// 조준 중이면 절반
	if(GetIsAiming())
	{
		DefaultRecoilZ *= 0.5f;
	}
	return DefaultRecoilZ;
}

void UGunActorComponent::PlayOwnerMontage(class UAnimMontage* OwnerMontage, float InRate)
{
	if(!OwnerCharacter || !OwnerMontage) return;
	
	if(UAnimInstance* OwnerAnimIns = OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		if(!OwnerAnimIns->Montage_IsPlaying(OwnerMontage))
		{
			// 몽타주를 배속(InRate)으로 실행
			OwnerAnimIns->Montage_Play(OwnerMontage, InRate);	
		}
	}
	
}

// 집탄 범위 조절
void UGunActorComponent::IncreaseSpreadRange(float Y, float Z)
{
	GunInfoStruct.SpreadY += Y;
	GunInfoStruct.SpreadZ += Z;
}

// 탄창 개수 증가
void UGunActorComponent::IncreaseMaxMagazine(int32 ChangeMagazine)
{
	GunInfoStruct.MaxMagazine += ChangeMagazine;
}

void UGunActorComponent::IncreaseShootInterval(float ChangeShootInterval)
{
	GunInfoStruct.ShootInterval += ChangeShootInterval;
}

void UGunActorComponent::IncreaseShotgunBulletNum(int32 ShotgunBullet)
{
	UE_LOG(LogTemp, Warning, TEXT("Parent's IncreaseShotgunBulletNum Function"));
}

void UGunActorComponent::SetRateReloadAnimMontage(float InRate)
{
	RateReloadMontage = InRate;
}

void UGunActorComponent::Start_ZoomIn()
{
	
}

void UGunActorComponent::Start_ZoomOut()
{
	
}

void UGunActorComponent::CameraShakeRecoil()
{
}
