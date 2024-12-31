// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking")
};

UCLASS()
class PUCKING_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void CheckPatrolTarget();
	AActor* ChoosePatrolTarget();
	void PatrolTimerFinished();
	void MoveToTarget(AActor* Target);
	bool InTargetRange(AActor* Target, float Radius);

	void Die();
	void CheckCombatTarget();
	void DirectionalHitReact(const FVector& ImpactPoint);

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);
protected:
	UPROPERTY()
	class AAIController* EnemyController;
	
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	AActor* PatrolTarget;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;
		
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMin = 5.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float WaitMax = 10.f;

	UPROPERTY(EditAnywhere)
	double PatrolRadius = 200.f;
	
	FTimerHandle PatrolTimer;

	UPROPERTY(EditAnywhere)
	class UPawnSensingComponent* PawnSensingComp;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
};