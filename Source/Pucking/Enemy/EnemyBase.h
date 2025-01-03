// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"


UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged")
};

UCLASS()
class PUCKING_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();
	
	UPROPERTY(EditAnywhere)
	class UPawnSensingComponent* PawnSensingComp;
	
	UPROPERTY(EditAnywhere)
	class UEnemyStatusComponent* StatusComp;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	//
	//Navigation
	//
	void CheckPatrolTarget();
	void CheckCombatTarget();
	AActor* ChoosePatrolTarget();
	void PatrolTimerFinished();
	void MoveToTarget(AActor* Target);
	bool InTargetRange(AActor* Target, float Radius);

	//
	//Take Hit
	//
	void Die();
	void DirectionalHitReact(const FVector& ImpactPoint);
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void HideHealthBar();
	void ShowHealthBar();
	
	//
	//Attack
	//
	void Attack();
	void PlayAttackMontage();
	void LoseInterest();
	void StartPatrolling();
	void ChaseTarget();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool CanAttack();
	
	//
	//Sense
	//
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

protected:
	UPROPERTY()
	class AAIController* EnemyController;

	UPROPERTY(BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	
	UPROPERTY(EditInstanceOnly, Category = "Combat")
	AActor* CombatTarget;
	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.5f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 1.f;
	
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;
	FTimerHandle PatrolTimer;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin = 2.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax = 4.f;
	
	UPROPERTY(EditAnywhere)
	float PatrolAcceptanceRadius = 200.f;
	UPROPERTY(EditAnywhere)
	float CombatRadius = 1000.f;
	UPROPERTY(EditAnywhere)
	float AttackRadius = 150.f;
	UPROPERTY(EditAnywhere)
	float WalkSpeed = 125.f;
	UPROPERTY(EditAnywhere)
	float RunSpeed = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackMontage;

	UPROPERTY()
	TSubclassOf<class UHealthBarComponent> HealthBarClass;

	UPROPERTY()
	UHealthBarComponent* HealthBarWidget;
};