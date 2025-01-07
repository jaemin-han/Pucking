// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Common/CommonEnum.h"
#include "StatusInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UStatusInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PUCKING_API IStatusInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//EnemyStatusComponent, PlayerStatusComponent�� ��ӹ���
	virtual void DamageCalculation() =0;
	virtual void GetDamage(EDamageType GetDamageType, float GetdamageAmount, float GetPenetration, const FHitResult& _hitRes) = 0;
	virtual void DamageProcessing(AActor* hitActor, const FHitResult& _hitRes) = 0;
};
