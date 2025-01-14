// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/StatusComponent.h"
#include "Interfaces/StatusInterface.h"
#include "World/PuckGameInstance.h"
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
	float EnemyHPToLevel = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemyShieldToLevel = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemyDamageToLevel = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemyStagger;

	//Use DamageProcessing
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PenetrationType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefenseAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StaggerAmount;

	UPROPERTY(EditAnywhere)
	class UPlayerStatusComponent* TargetPlayerComp = nullptr;
	UPROPERTY(EditAnywhere)
	class ACharacter* TargetPlayer;

	UPROPERTY(EditAnywhere)
	UPuckGameInstance* PuckGameInstance;
public:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	virtual void DamageCalculation() override;
	UFUNCTION(BlueprintCallable)
	virtual void GetDamage(EDamageType GetDamageType, float GetdamageAmount, float Penetration, float GetStaggerValue, const FHitResult& _hitRes) override;
	virtual void DamageProcessing(AActor* hitActor, const FHitResult& _hitRes) override;
	virtual void Die() override;
};
