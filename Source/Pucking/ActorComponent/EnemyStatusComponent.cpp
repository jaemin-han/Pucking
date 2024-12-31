// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/EnemyStatusComponent.h"

void UEnemyStatusComponent::DamageProcessing(AActor* hitActor)
{
	//EnemyStatus DamageProcessing
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

void UEnemyStatusComponent::GetDamage(float damageAmount, float Penetration)
{
}
