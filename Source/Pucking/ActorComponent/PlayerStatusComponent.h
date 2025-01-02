// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/StatusComponent.h"
#include "Interfaces/StatusInterface.h"
#include "PlayerStatusComponent.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API UPlayerStatusComponent : public UStatusComponent, public IStatusInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PenetrationType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefenseAmount;

	UPROPERTY(EditAnywhere)
	class UEnemyStatusComponent* TargetEnemy;
public:
	//줄 데미지 계산
	UFUNCTION(BlueprintCallable)
	virtual void DamageCalculation() override;
	//데미지 받을 때 호출
	UFUNCTION(BlueprintCallable)
	virtual void GetDamage(EDamageType GetDamageType, float GetdamageAmount, float Penetration) override;
	virtual void DamageProcessing(AActor* hitActor) override;
};
