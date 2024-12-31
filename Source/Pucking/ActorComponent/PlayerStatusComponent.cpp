// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/PlayerStatusComponent.h"

void UPlayerStatusComponent::DamageProcessing(AActor* hitActor)
{
	//PlayerStatus DamageProcessing
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

void UPlayerStatusComponent::GetDamage(float damageAmount, float Penetration)
{
}
