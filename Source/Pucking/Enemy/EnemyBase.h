// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "World/PuckGameInstance.h"
#include "EnemyBase.generated.h"


UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	EES_Dead UMETA(DisplayName = "Dead"),
	EES_BlackHole UMETA(DisplayName = "BlackHole"),
	EES_Patrolling UMETA(DisplayName = "Patrolling"),
	EES_Chasing UMETA(DisplayName = "Chasing"),
	EES_Attacking UMETA(DisplayName = "Attacking"),
	EES_Engaged UMETA(DisplayName = "Engaged"),
	EES_Hit UMETA(DisplayName = "Hit"),
	EES_NoState UMETA(DisplayName = "NoState")
};
enum EDeathPose
{
	EDP_Death1 UMETA(DisplayName = "Death1"),
	EDP_MAX UMETA(DisplayName = "DefaultMax")
};

UCLASS()
class PUCKING_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()
	
	///////Functions//////////
public:
	AEnemyBase();
	virtual void Tick(float DeltaTime) override;
	void GetHit(const FHitResult& HitResult, const float StaggerTime);
	UFUNCTION()
	void OnCombatCompAttachment(UStaticMeshComponent* TargetMeshComp, USceneComponent* BoxTraceStart, USceneComponent* BoxTraceEnd);
protected:
	virtual void BeginPlay() override;
	
private:
	//
	//Navigation
	//
	void CheckPatrolTarget();
	void CheckCombatTarget();
	AActor* ChoosePatrolTarget();
	void MoveToTarget(AActor* Target);
	void StartPatrolling();
	bool InTargetRange(AActor* Target, float Radius);
	void StopMovement(const float Time);
	
	//
	//Take Hit
	//
public:
	void Revive();
	UFUNCTION(BlueprintCallable)
	void Die();
	UFUNCTION(BlueprintCallable)
	void HitByExplosion(FVector ExplosionLocation);
	UFUNCTION(BlueprintCallable)
	void HitByBlackHole(FVector BlackHoleLocation);
	void SuckedByBlackHole();
	UFUNCTION(BlueprintCallable)
	void SetHealthShieldBar();

private:
	void DirectionalHitReact(const FVector& ImpactPoint);
	void HideHealthBar();
	void ShowHealthBar();
	int32 PlayDeathMontage();
	
	//
	//Attack
	//
	void Attack();
	int32 PlayAttackMontage();
	void LoseInterest();
	void ChaseTarget();
	void StartAttackTimer();
	void ClearAttackTimer();
	bool CanAttack();
	UFUNCTION(BlueprintCallable)
	void AttackEnd();
	UFUNCTION(BlueprintCallable)
	void LongRangeAttack();
	void LookAtTarget(AActor* Target);
	
	//
	//Sense
	//
	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

	//
	//Animation
	//	
	void PlayMontageSection(UAnimMontage* Montage, const FName& SectionName);
	int32 PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames);

	//
	//Sound
	//	
	void PlaySound(USoundBase* Sound, const FVector& Location);

	//
	//Object Pool
	//	
public:
	void Activate();
	void Deactivate();
	bool IsActive() const { return bIsActive; }
	void Initialize(FVector SpawnLocation);
	void ReturnPool();
	void ReturnAfterDelay();
	void EnemyBaseStatusInit();
private:
	void DropItems();
	void SpawnNiagara();
	

	//////////Variables//////////////
public:

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsDead = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsBeingSucked = false;
	
	bool bIsHitByPlayer = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<FName> AttackMontageSections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* DeathMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	TArray<FName> DeathMontageSections;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* TakeHitMontage;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TArray<USoundBase*>HitSounds;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* ScreamSound;

	UPROPERTY(EditAnywhere, Category = "Effects")
	USoundBase* DeathSound;

	UPROPERTY(EditAnywhere, Category = "Effects")
	TArray<class UNiagaraSystem*> BloodEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AProjectileBase> EnemyProjectileClass;
private:
	UPROPERTY(EditAnywhere)
	class UPawnSensingComponent* PawnSensingComp;
	
	UPROPERTY(EditAnywhere)
	class UEnemyStatusComponent* StatusComp;

	UPROPERTY(EditAnywhere, Category = "CloseCombat")
	class UCloseCombatComponent* CloseCombatComp;

	UPROPERTY(EditAnywhere)
	UArrowComponent* FireArrowComp;
	
	UPROPERTY(EditAnywhere)
	class UHealthBarComponent* HealthBarWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HealthBarClass;
	
	UPROPERTY()
	class AAIController* EnemyController;
	
	UPROPERTY(EditInstanceOnly, Category = "Combat")
	AActor* CombatTarget;
	FTimerHandle AttackTimer;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMin = 0.2f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMax = 0.4f;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AttackMontageTime = 0.7f;
	UPROPERTY(EditAnywhere)
	float CombatRadius = 1000.f;
	UPROPERTY(EditAnywhere)
	float AttackRadius = 150.f;
	
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	AActor* PatrolTarget;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	TArray<AActor*> PatrolTargets;
	FTimerHandle PatrolTimer;
	UPROPERTY(EditInstanceOnly, Category = "AI Navigation")
	FVector BlackHoleTarget;
	FTimerHandle BlackHoleTimer;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMin = 2.f;
	UPROPERTY(EditAnywhere, Category = "AI Navigation")
	float PatrolWaitMax = 4.f;;
	UPROPERTY(EditAnywhere)
	float PatrolAcceptanceRadius = 200.f;
	UPROPERTY()
	FTimerHandle RotationTimer;
	
	UPROPERTY(EditAnywhere)
	float WalkSpeed = 212.5f;
	UPROPERTY(EditAnywhere)
	float RunSpeed = 425.f;
	UPROPERTY(EditAnywhere)
	float BlackHoleSpeed = 1000.f;
	UPROPERTY(EditAnywhere)
	float DeathLifeSpan = 5.f;

	TEnumAsByte<EDeathPose> DeathPose;


	///////Object Pool ///////
	bool bIsActive;
	FTimerHandle DeathAnimHandle;
	UPuckGameInstance* PuckGameInstance;

	///////Spawn Niagara//////
	UPROPERTY(EditAnywhere, Category = "SpawnEffect")
	class UNiagaraSystem* SpawnNiagaraTemplate;
	UPROPERTY(EditAnywhere, Category = "SpawnEffect")
	class UNiagaraComponent* SpawnNiagaraComp;

	class AEnemySpawnerTest* EnemySpawner;
};