// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/EnemyStatusComponent.h"
#include "ActorComponent/PlayerStatusComponent.h"
#include "Enemy/EnemyBase.h"
#include "GameFramework/Character.h"
#include "World/LevelTestPuckingGameMode.h"
#include "Kismet/GameplayStatics.h"

void UEnemyStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	MaxHP = 10;
	MaxShield = 10;
	CurMaxHP = MaxHP;
	CurMaxShield = MaxShield;

	//EnemyHPToLevel = MaxHP * Level;
	EnemyHPToLevel = CurMaxHP * 2;
	EnemyShieldToLevel = CurMaxShield * 2;

	RemainHP = EnemyHPToLevel;
	RemainShield = EnemyShieldToLevel;
}

void UEnemyStatusComponent::DamageCalculation()
{
	//EnemyStatus DamageCalculation
	switch (CommonDamageType)
	{
	case EDamageType::Physical:
		PenetrationType = CurPhysicalPenetration;
		break;
	case EDamageType::Fire:
		PenetrationType = CurFirePenetration;
		break;
	case EDamageType::Ice:
		PenetrationType = CurIcePenetration;
		break;
	default:
		break;
	}
	//PlayerStatus DamageCalculation
	float CriticalRange = FMath::FRandRange(0.0f, 100.0f);
	if (CurCriticalChance >= CriticalRange)
	{
		DamageAmount = CurDamage * CurCriticalMultipier;

	}
	else if (CurCriticalChance < CriticalRange)
	{
		DamageAmount = CurDamage;
	}
	UE_LOG(LogTemp, Warning, TEXT("[%s] 's DamageCalculating Success!!"), *GetOwner()->GetName());
	
}

void UEnemyStatusComponent::GetDamage(EDamageType GetDamageType, float GetdamageAmount, float Penetration, const FHitResult& _hitRes)
{
	RemainShield -= GetdamageAmount;
	//실드가 없는 상태면
	if (RemainShield <= 0)
	{
		//나이아가라 끄기
		//NiagaraComp->Deactivate();
		//NiagaraComp->SetVisibility(false);

		RemainShield = 0;
		//체력 처리로 이동
		switch (GetDamageType)
		{
		case EDamageType::Physical:
			DefenseAmount = CurPhysicalDefense - Penetration;
			if (DefenseAmount >= GetdamageAmount)
			{
				RemainHP -= 1;
			}
			RemainHP = RemainHP - (GetdamageAmount - DefenseAmount);
			break;
		case EDamageType::Fire:
			DefenseAmount = CurFireDefense - Penetration;
			if (DefenseAmount >= GetdamageAmount)
			{
				RemainHP -= 1;
			}
			RemainHP = RemainHP - (GetdamageAmount - DefenseAmount);
			break;
		case EDamageType::Ice:
			DefenseAmount = CurIceDefense - Penetration;
			if (DefenseAmount >= GetdamageAmount)
			{
				RemainHP -= 1;
			}
			RemainHP = RemainHP - (GetdamageAmount - DefenseAmount);
			break;
		default:
			break;
		}
		/*if (RemainHP <= 0)
		{
			AGameModeBase* GameMode = UGameplayStatics::GetGameMode(GetWorld());
			if (!GameMode) return;

			ALevelTestPuckingGameMode* MyGameMode = Cast<ALevelTestPuckingGameMode>(GameMode);
			if (MyGameMode)
			{
				MyGameMode->LevelCheck();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("Can't Call LevelCheck Function!!"));
			}
		}*/
	}
	
	AEnemyBase* OwnerEnemy = Cast<AEnemyBase>(GetOwner());
	if(OwnerEnemy)
	{
		OwnerEnemy->GetHit(_hitRes);
	}
	
	//피해를 받으면 회복중이던 타이머 멈춤(삭제)
	GetOwner()->GetWorld()->GetTimerManager().ClearTimer(RecoverySpeedTimer);

	//데미지 받고 3초 후 실드 회복 시작
	GetOwner()->GetWorld()->GetTimerManager().SetTimer(RecoveryDelayTimer, this, &UStatusComponent::ShieldRecovery, 3.0f, false);

	//UE_LOG(LogTemp, Warning, TEXT("DamageAmount : %f, DefenseAmount : %f, Penetration : %f, RemainHP : %f, TotalGetDamage : %f"), DamageAmount, DefenseAmount, PenetrationType, RemainHP, GetdamageAmount - DefenseAmount);
	UE_LOG(LogTemp, Warning, TEXT("<%s> Get Damage!!"), *GetOwner()->GetName());
}


void UEnemyStatusComponent::DamageProcessing(AActor* hitActor, const FHitResult& _hitRes)
{
	if (IsValid(hitActor))
	{
		TargetPlayerComp = hitActor->FindComponentByClass<UPlayerStatusComponent>();
		if (TargetPlayerComp)
		{
			//내가 줄 데미지 계산하고
			DamageCalculation();
			//맞은 타겟의 EnemyStatusComponent의 GetDamage를 실행
			TargetPlayerComp->GetDamage(CommonDamageType, DamageAmount, PenetrationType, _hitRes);
			//GetDamage(CommonDamageType, DamageAmount, PenetrationType);
		}
		else return;
	}
	else return;
}

void UEnemyStatusComponent::Die()
{
	Owner->Destroy();
}
