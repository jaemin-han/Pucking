// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/EnemyStatusComponent.h"
#include "ActorComponent/PlayerStatusComponent.h"
#include "Enemy/EnemyBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UEnemyStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	PuckGameInstance = Cast<UPuckGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//MaxHP = 10;
	//MaxShield = 10;
	EnemyStatInit();
	
}

void UEnemyStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//DrawDebugString(GetWorld(), GetOwner()->GetActorLocation() - FVector(0, 0, 20), FString::Printf(TEXT("HP : %.1f"), RemainHP), 0, FColor::Red, 0.005f, false, 2.0f);

	//DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), FString::Printf(TEXT("SHIELD : %.1f"), RemainShield), 0, FColor::White, 0.005f, false, 2.0f);
	//DrawDebugString(GetWorld(), GetOwner()->GetActorLocation() + FVector(0, 0, 20), FString::Printf(TEXT("MonsterLevel : %d"), PuckGameInstance->LevelNum), 0, FColor::Blue, 0.005f, false, 2.0f);
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
	StaggerAmount = CurStaggerValue;
	
}

void UEnemyStatusComponent::GetDamage(EDamageType GetDamageType, float GetdamageAmount, float Penetration, float GetStaggerValue, const FHitResult& _hitRes)
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
			else if (DefenseAmount < GetdamageAmount)
			{
				RemainHP = RemainHP - (GetdamageAmount - DefenseAmount);
			}
			break;
		case EDamageType::Fire:
			DefenseAmount = CurFireDefense - Penetration;
			if (DefenseAmount >= GetdamageAmount)
			{
				RemainHP -= 1;
			}
			else if (DefenseAmount < GetdamageAmount)
			{
				RemainHP = RemainHP - (GetdamageAmount - DefenseAmount);
			}
			break;
		case EDamageType::Ice:
			DefenseAmount = CurIceDefense - Penetration;
			if (DefenseAmount >= GetdamageAmount)
			{
				RemainHP -= 1;
			}
			else if (DefenseAmount < GetdamageAmount)
			{
				RemainHP = RemainHP - (GetdamageAmount - DefenseAmount);
			}
			break;
		default:
			break;
		}


		/*if (RemainHP <= 0)
		{
			PuckGameInstance->LevelCheck();
		}*/


	}
	//	//니들이 만들어와
	//	//[Enemy경직 방어도] - [Player 경직 수치] < 0 인 경우 경직이 해당 시간 만큼 실행
	if ((CurStaggerResistance - GetStaggerValue) < 0)
	{
		//Here You Are
	}
	float StaggerTime = 1.f;
	
	AEnemyBase* OwnerEnemy = Cast<AEnemyBase>(GetOwner());
	if(OwnerEnemy)
	{
		OwnerEnemy->GetHit(_hitRes, StaggerTime);
	}
	
	//피해를 받으면 회복중이던 타이머 멈춤(삭제)
	GetOwner()->GetWorld()->GetTimerManager().ClearTimer(RecoverySpeedTimer);

	//데미지 받고 3초 후 실드 회복 시작
	GetOwner()->GetWorld()->GetTimerManager().SetTimer(RecoveryDelayTimer, this, &UStatusComponent::ShieldRecovery, 3.0f, false);

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
			TargetPlayerComp->GetDamage(CommonDamageType, DamageAmount, PenetrationType, StaggerAmount, _hitRes);
			//GetDamage(CommonDamageType, DamageAmount, PenetrationType);
		}
		else return;
	}
	else return;
}

void UEnemyStatusComponent::Die()
{
	//Owner->Destroy();
	//UE_LOG(LogTemp, Warning, TEXT("EnemyStatusComponent::Die"));
}

void UEnemyStatusComponent::EnemyStatInit()
{
	PuckGameInstance = Cast<UPuckGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	ResetStaticStatus();
	CurMaxHP = CurMaxHP * PuckGameInstance->CurrentRow.NormalEnemyHPIncreaseRate;
	CurMaxShield = EnemyShieldToLevel;
	CurDamage = EnemyDamageToLevel;
	int32 EnemyDamageTypeRandom = FMath::RandRange(0, 2);
	if (EnemyDamageTypeRandom == 0)
	{
		CommonDamageType = EDamageType::Physical;
	}
	else if (EnemyDamageTypeRandom == 1)
	{
		CommonDamageType = EDamageType::Fire;
	}
	else if (EnemyDamageTypeRandom == 2)
	{
		CommonDamageType = EDamageType::Ice;
	}
	//UE_LOG(LogTemp, Warning, TEXT("DamageType %s"), *UEnum::GetDisplayValueAsText(CommonDamageType).ToString());
	//EnemyHPToLevel = MaxHP * Level;
	//EnemyHPToLevel = CurMaxHP * 1;
	//EnemyShieldToLevel = CurMaxShield * 1;
	//RemainHP = EnemyHPToLevel;
	//RemainShield = EnemyShieldToLevel;

	RemainHP = CurMaxHP;
	RemainShield = CurMaxShield;
}
