// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/StatusComponent.h"
#include "Interfaces/StatusInterface.h"
#include "EnemyStatusComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class PUCKING_API UEnemyStatusComponent : public UStatusComponent, public IStatusInterface
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
	class UPlayerStatusComponent* TargetPlayer;
public:
	UFUNCTION(BlueprintCallable)
	virtual void DamageCalculation() override;
	UFUNCTION(BlueprintCallable)
	virtual void GetDamage(EDamageType GetDamageType, float GetdamageAmount, float Penetration) override;
	virtual void DamageProcessing(AActor* hitActor) override;
};
