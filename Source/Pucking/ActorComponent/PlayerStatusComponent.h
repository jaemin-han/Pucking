// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/StatusComponent.h"
#include "Interfaces/StatusInterface.h"
#include "PlayerStatusComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
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
	class UEnemyStatusComponent* TargetEnemyComp = nullptr;

	UPROPERTY(EditAnywhere)
	class ACharacter* TargetEnemy;
public:
	//줄 데미지 계산
	UFUNCTION(BlueprintCallable)
	virtual void DamageCalculation() override;
	//데미지 받을 때 호출
	UFUNCTION(BlueprintCallable)
	virtual void GetDamage(EDamageType GetDamageType, float GetdamageAmount, float Penetration) override;
	UFUNCTION(BlueprintCallable)
	virtual void DamageProcessing(AActor* hitActor, FHitResult _hitRes) override;
};
