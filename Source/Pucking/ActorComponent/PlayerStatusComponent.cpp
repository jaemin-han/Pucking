// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/PlayerStatusComponent.h"

void UPlayerStatusComponent::DamageProcessing(float DamageAmount, EDamageType damageType, float Penetration)
{
	//PlayerStatus DamageProcessing
	float RandomFloat = FMath::FRand();
	if (CurCriticalChance >= RandomFloat)
	{
		CurDamage = CurDamage * CurCriticalMultipier;

	}
	else if (CurCriticalChance < RandomFloat)
	{
		CurDamage = CurDamage;
	}

}
