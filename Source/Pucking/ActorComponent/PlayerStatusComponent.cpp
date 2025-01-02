// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/PlayerStatusComponent.h"
#include "ActorComponent/EnemyStatusComponent.h"
#include "GameFramework/Character.h"

//줄 데미지 계산
void UPlayerStatusComponent::DamageCalculation()
{
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
	UE_LOG(LogTemp, Warning, TEXT("%s 's DamageAmount"), *GetOwner()->GetName());
	
}

//방어력 계산해서 받는 데미지 결정
void UPlayerStatusComponent::GetDamage(EDamageType GetDamageType, float GetdamageAmount, float Penetration)
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

	}

	//피해를 받으면 회복중이던 타이머 멈춤(삭제)
	GetOwner()->GetWorld()->GetTimerManager().ClearTimer(RecoverySpeedTimer);

	//데미지 받고 3초 후 실드 회복 시작
	GetOwner()->GetWorld()->GetTimerManager().SetTimer(RecoveryDelayTimer, this, &UStatusComponent::ShieldRecovery, 3.0f, false);
	
	//UE_LOG(LogTemp, Warning, TEXT("DamageAmount : %f, DefenseAmount : %f, Penetration : %f, RemainHP : %f, TotalGetDamage : %f"), DamageAmount, DefenseAmount, PenetrationType, RemainHP, GetdamageAmount - DefenseAmount);
	UE_LOG(LogTemp, Warning, TEXT("<%s> Get Damage!!"), *GetOwner()->GetName());
}

//맞은 Actor를 받아서 데미지 처리 실행
void UPlayerStatusComponent::DamageProcessing(AActor* hitActor)
{
	//맞은 타겟에 EnemyStatusComponent가 있으면
	if (IsValid(hitActor))
	{
		TargetEnemy = Cast<ACharacter>(hitActor);
		TargetEnemyComp = TargetEnemy->FindComponentByClass<UEnemyStatusComponent>();

		if (TargetEnemy)
		{
			//내가 줄 데미지 계산하고
			DamageCalculation();
			//맞은 타겟의 EnemyStatusComponent의 GetDamage를 실행
			TargetEnemyComp->GetDamage(CommonDamageType, DamageAmount, PenetrationType);
			//GetDamage(CommonDamageType, DamageAmount, PenetrationType);
		}
		else return;
	}
	else return;
}
