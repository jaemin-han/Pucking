// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/EnemyStatusComponent.h"
#include "ActorComponent/PlayerStatusComponent.h"

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
	
	
}

void UEnemyStatusComponent::GetDamage(EDamageType GetDamageType, float GetdamageAmount, float Penetration)
{
	switch (GetDamageType)
	{
	case EDamageType::Physical:
		DefenseAmount = CurPhysicalDefense - Penetration;
		if (DefenseAmount <= 0)
		{
			DefenseAmount = 1;
		}
		RemainHP = RemainHP - (GetdamageAmount - DefenseAmount);
		break;
	case EDamageType::Fire:
		DefenseAmount = CurFireDefense - Penetration;
		if (DefenseAmount <= 0)
		{
			DefenseAmount = 1;
		}
		RemainHP = RemainHP - (GetdamageAmount - DefenseAmount);
		break;
	case EDamageType::Ice:
		DefenseAmount = CurIceDefense - Penetration;
		if (DefenseAmount <= 0)
		{
			DefenseAmount = 1;
		}
		RemainHP = RemainHP - (GetdamageAmount - DefenseAmount);
		break;
	default:
		break;
	}
}

void UEnemyStatusComponent::DamageProcessing(AActor* hitActor)
{
	TargetPlayer = hitActor->FindComponentByClass<UPlayerStatusComponent>();
	if (TargetPlayer)
	{
		DamageCalculation();
		TargetPlayer->GetDamage(CommonDamageType, DamageAmount, PenetrationType);
	}
	else return;
}
