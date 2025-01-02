// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/StatusComponent.h"
#include "Interfaces/StatusInterface.h"
#include "EnemyStatusComponent.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API UEnemyStatusComponent : public UStatusComponent, public IStatusInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	float DamageAmount;
	UPROPERTY(EditAnywhere)
	float PenetrationType;
	UPROPERTY(EditAnywhere)
	float DefenseAmount;

	UPROPERTY(EditAnywhere)
	class UPlayerStatusComponent* TargetPlayer;
public:
	virtual void DamageCalculation() override;
	virtual void GetDamage(EDamageType GetDamageType, float GetdamageAmount, float Penetration) override;
	virtual void DamageProcessing(AActor* hitActor) override;
};
