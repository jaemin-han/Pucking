// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/GunActorComponent.h"

#include "NiagaraComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/SlateWrapperTypes.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "Character/PuckAnimInstance.h"
#include "Character/PuckingCharacter.h"

// Sets default values for this component's properties
UGunActorComponent::UGunActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	bIsShootAble = true;

	ComponentTags.Add(FName("GunActorComponent"));
}


// Called when the game starts
void UGunActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// 초기화
	//InitActorComponent();
}


// Called every frame
void UGunActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// 초기화
void UGunActorComponent::InitActorComponent()
{
	// Owner Actor를 먼저 확인
	if(GetOwner())
	{
		// Montage 재생할 때 필요한 Character로 캐싱
		if(Cast<ACharacter>(GetOwner()))
		{
			OwnerCharacter = Cast<ACharacter>(GetOwner());
			if(OwnerCharacter)
			{
				// Widget Delegate
				if(APuckingCharacter* PuckingCharacter = Cast<APuckingCharacter>(GetOwner()))
				{
					if(!PuckingCharacter->OnToggleWidget.IsAlreadyBound(this, &UGunActorComponent::ToggleWidget))
					{
						PuckingCharacter->OnToggleWidget.AddDynamic(this, &UGunActorComponent::ToggleWidget);
					}
				}
				
				// Get Fsm Delegate
				if(OwnerCharacter->GetMesh() &&OwnerCharacter->GetMesh()->GetAnimInstance())
				{
					UPuckAnimInstance* AnimIns = Cast<UPuckAnimInstance>(OwnerCharacter->GetMesh()->GetAnimInstance());
					if(AnimIns)
					{
						if(!AnimIns->OnChangeFsm.IsAlreadyBound(this, &UGunActorComponent::GetCurrentFsm))
						{
							AnimIns->OnChangeFsm.AddDynamic(this, &UGunActorComponent::GetCurrentFsm);
						}
					}
				}
			}
		}
	}
}

void UGunActorComponent::SetDefaultGunInfoStruct(FName TableRows)
{
	// DataTable 정보를 세팅
	if (GunInfoDataTable)
	{
		FGunInfoStruct* DT_GunData = GunInfoDataTable->FindRow<FGunInfoStruct>(FName(TableRows), TEXT(""));
		
		GunInfoStruct.GunType = DT_GunData->GunType;
		GunInfoStruct.Magazine = 0;
		GunInfoStruct.MaxMagazine = DT_GunData->MaxMagazine;
		GunInfoStruct.DefaultDamage = DT_GunData->DefaultDamage;
		GunInfoStruct.SpreadX = DT_GunData->SpreadX;
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
			// todo: 재민 수정
			SkeletalMeshComponent->SetRelativeTransform(ActorTransform);
			SkeletalMeshComponent->SetSkeletalMesh(GunSkeletalMesh);
			SkeletalMeshComponent->AttachToComponent(TargetSkeletalMeshComp, FAttachmentTransformRules::KeepRelativeTransform, SocketName);

			GetOwner()->AddInstanceComponent(SkeletalMeshComponent);
			
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

	// 데미지 타입에 따른 Muzzle Effect
	// Muzzle Effect 위치
	FVector MuzzleLoc = SkeletalMeshComponent->GetSocketLocation(MuzzleSocketName);

	// Muzzle Effect Transform
	FTransform MuzzleTransform = FTransform(SkeletalMeshComponent->GetSocketRotation(MuzzleSocketName), MuzzleLoc);

	// MuzzleTransform 으로 ForwardVector를 계산
	FVector MuzzleForwardVector = MuzzleTransform.GetRotation().GetForwardVector();

	MuzzleLoc -= MuzzleForwardVector * MuzzleOffset;
	

	// 나이아가라 Component
	UNiagaraComponent* NiagaraComp = nullptr;
	
	if(DamageType == EDamageType::Fire)
	{
		/*if(MuzzleParticleFire)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleParticleFire, MuzzleLoc, FRotator(0, 0, 0));
		}*/
		if(MuzzleNiagaraFire)
		{
			NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleNiagaraFire, MuzzleLoc);
		}
	}
	else if(DamageType == EDamageType::Ice)
	{
		/*if(MuzzleParticleIce)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleParticleIce, MuzzleLoc, FRotator(0, 0, 0));	
		}*/
		if(MuzzleNiagaraIce)
		{
			NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleNiagaraIce, MuzzleLoc);
		}
	}
	else
	{
		/*if(MuzzleParticleNormal)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleParticleNormal, MuzzleLoc, FRotator(0, 0, 0));
		}*/
		if(MuzzleNiagaraNormal)
		{
			NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), MuzzleNiagaraNormal, MuzzleLoc);
		}
	}

	if(NiagaraComp)
	{
		FTimerHandle ClearTimer;
		GetWorld()->GetTimerManager().SetTimer(ClearTimer, [NiagaraComp]()
		{
			NiagaraComp->Deactivate();
		}, 0.2f, false);
	}
	
	if(OnFireDelegate.IsBound())
	{
		OnFireDelegate.Broadcast(GunInfoStruct.Magazine);
	}
}

void UGunActorComponent::Reload()
{
	/*// Delegate에 바운드 되어있는지 확인
	if(OnRemainAmmo.IsBound())
	{
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
	}*/
}

void UGunActorComponent::ToggleWidget(bool bIsOnWidget)
{
	/*UE_LOG(LogTemp, Warning, TEXT("name : %s, CurWeaponType : %s, PlayerType : %s")
		, *this->GetName(), *UEnum::GetValueAsString(GetWeaponType()), *UEnum::GetValueAsString(PlayerWeaponType));*/
	if(bIsOnWidget)
	{
		if(CrosshairWidget)
		{
			// 다시 UI를 보이게 할 때 타입에 맞는 UI만 Visible
			if(PlayerWeaponType == GetWeaponType())
			{
				CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
	else
	{
		if(CrosshairWidget)
		{
			CrosshairWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
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
		SkeletalMeshComponent->SetVisibility(true);
		if(BP_GunActor)
		{
			BP_GunActor->SetActorHiddenInGame(false);
		}
		if(CrosshairWidget)
		{
			CrosshairWidget->SetVisibility(ESlateVisibility::Visible);
		}
	}
	else
	{
		SkeletalMeshComponent->SetVisibility(false);
		if(BP_GunActor)
		{
			BP_GunActor->SetActorHiddenInGame(true);
		}
		if(CrosshairWidget)
		{
			CrosshairWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	
}

FDelegateHandle UGunActorComponent::DelegateFireComplete(const TDelegate<void(int32)>& Delegate)
{
	return OnFireDelegate.Add(Delegate);
}

FDelegateHandle UGunActorComponent::DelegateReloadComplete(const TDelegate<void(int32)>& Delegate)
{
	return OnReloadDelegate.Add(Delegate);
}


void UGunActorComponent::SetIsAiming(bool CurrentAiming)
{
	bIsAiming = CurrentAiming;
}

bool UGunActorComponent::GetIsAiming() const
{
	return bIsAiming;
}

bool UGunActorComponent::IsCurWeaponType(EWeaponType CurWeaponType)
{
	// Visibility Check
	SetCurrentOwnerWeaponType(CurWeaponType);
	
	// Interface에서 판단할 때 사용
	bool IsThisWeaponType = (CurWeaponType == WeaponType);
	SetActive(IsThisWeaponType);
	
	return IsThisWeaponType;
}

TArray<FInputParameter> UGunActorComponent::ReturnInputParameter()
{
	return InputParameters;
}


float UGunActorComponent::GetSpreadXRange()
{
	// DataTable에서 기본 반동값 가져옴
	float DefaultRecoilX = GunInfoStruct.SpreadX;

	// 조준 중이면 절반
	if(GetIsAiming())
	{
		DefaultRecoilX *= GunInfoStruct.ModifyZoomRecoil;
	}
	return DefaultRecoilX;
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
		DefaultRecoilZ *= GunInfoStruct.ModifyZoomRecoil;
	}
	return DefaultRecoilZ;
}

void UGunActorComponent::GetCurrentFsm(ECharacterFSM TargetFsm)
{
	if(TargetFsm == ECharacterFSM::Zoom)
	{
		Start_ZoomOut();
	}
}

bool UGunActorComponent::IsCanPlayMontageState(ECharacterMontage TargetMontage)
{
	bool IsCanPlay = false;
	
	if(OwnerCharacter && OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		IMontageFSMInterface* OwnerAnimIns = Cast<IMontageFSMInterface>(OwnerCharacter->GetMesh()->GetAnimInstance());
		if(OwnerAnimIns)
		{
			IsCanPlay = OwnerAnimIns->CheckFsmByMontage(TargetMontage);
		}
	}
	
	return IsCanPlay;
}

bool UGunActorComponent::IsCanChangeState(ECharacterFSM TargetFsm)
{
	bool IsCanChange = false;
	
	if(OwnerCharacter && OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		IMontageFSMInterface* OwnerAnimIns = Cast<IMontageFSMInterface>(OwnerCharacter->GetMesh()->GetAnimInstance());
		if(OwnerAnimIns)
		{
			IsCanChange = OwnerAnimIns->CheckFsmByEnum(TargetFsm);
		}
	}
	
	return IsCanChange;
}

void UGunActorComponent::ChangeState(ECharacterFSM TargetFsm)
{
	if(!OwnerCharacter) return;
	
	if(OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		IMontageFSMInterface* OwnerAnimIns = Cast<IMontageFSMInterface>(OwnerCharacter->GetMesh()->GetAnimInstance());
		if(OwnerAnimIns)
		{
			OwnerAnimIns->ReceiveFsm(TargetFsm);
		}
	}
}

void UGunActorComponent::PlayOwnerMontage(ECharacterMontage TargetMontage, float InRate)
{
	if(!OwnerCharacter) return;
	
	if(OwnerCharacter->GetMesh()->GetAnimInstance())
	{
		IMontageFSMInterface* OwnerAnimIns = Cast<IMontageFSMInterface>(OwnerCharacter->GetMesh()->GetAnimInstance());
		if(OwnerAnimIns)
		{
			OwnerAnimIns->ReceiveMontageState(TargetMontage, InRate);
		}
	}
}

// 집탄 범위 조절
void UGunActorComponent::DecreaseSpreadRange()
{
}

// 탄창 개수 증가
void UGunActorComponent::IncreaseMaxMagazine(/*int32 ChangeMagazine*/)
{
	//GunInfoStruct.MaxMagazine += ChangeMagazine;
}

EWeaponType UGunActorComponent::GetWeaponType()
{
	return WeaponType;
}

int32 UGunActorComponent::GetMaxMagazine()
{
	return GunInfoStruct.MaxMagazine;
}

int32 UGunActorComponent::GetCurMagazine()
{
	return GunInfoStruct.Magazine;
}

void UGunActorComponent::SetShootInterval(/*float ChangeShootInterval*/)
{
	//GunInfoStruct.ShootInterval += ChangeShootInterval;
}

void UGunActorComponent::IncreaseBulletNum(/*int32 BulletNum*/)
{
	//UE_LOG(LogTemp, Warning, TEXT("Parent's IncreaseShotgunBulletNum Function"));
}

void UGunActorComponent::SetRateReloadAnimMontage(/*float InRate*/)
{
	//RateReloadMontage = InRate;
}
