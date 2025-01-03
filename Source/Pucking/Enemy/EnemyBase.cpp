// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"


#include "GameFramework/CharacterMovementComponent.h"

#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"

#include "Character/PuckingCharacter.h"
#include "ActorComponent/EnemyStatusComponent.h"
#include "UI/Enemy/HealthBarComponent.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	PawnSensingComp->SightRadius = 4000.f;
	PawnSensingComp->SetPeripheralVisionAngle(45.f);

	StatusComp = CreateDefaultSubobject<UEnemyStatusComponent>("StatusComp");
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(EnemyState == EEnemyState::EES_Dead) return;
	if(EnemyState > EEnemyState::EES_Patrolling)
	{
		CheckCombatTarget();
	}
	else
	{
		CheckPatrolTarget();
	}
}


void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	
	EnemyController = Cast<AAIController>(GetController());
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemyBase::PawnSeen);
	
	MoveToTarget(PatrolTarget);
}

void AEnemyBase::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolAcceptanceRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(WaitMin, WaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemyBase::PatrolTimerFinished, WaitTime);
	}
}

void AEnemyBase::PatrolTimerFinished()
{
	MoveToTarget(PatrolTarget);
}

void AEnemyBase::MoveToTarget(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(60.f);
	EnemyController->MoveTo(MoveRequest);
}

AActor* AEnemyBase::ChoosePatrolTarget()
{
	TArray<AActor*> ValidTargets;
	for (AActor* Target : PatrolTargets)
	{
		if (Target != PatrolTarget)
		{
			ValidTargets.AddUnique(Target);
		}
	}
	const int32 NumPatrolTargets = ValidTargets.Num();
	if (NumPatrolTargets > 0)
	{
		const int32 TargetSelection = FMath::RandRange(0, NumPatrolTargets - 1);
		return ValidTargets[TargetSelection];
	}
	return nullptr;
}

bool AEnemyBase::InTargetRange(AActor* Target, float Radius)
{
	if (Target == nullptr) return false;
	const double DistanceToTarget = (Target->GetActorLocation() - GetActorLocation()).Size();
	return DistanceToTarget <= Radius;
}

void AEnemyBase::PawnSeen(APawn* SeenPawn)
{
	const bool bShouldChaseTarget =
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Chasing &&
		EnemyState < EEnemyState::EES_Attacking &&
		SeenPawn->ActorHasTag(FName("Player"));
	if(bShouldChaseTarget)
	{
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		CombatTarget = SeenPawn;
		ChaseTarget();
	}
}

void AEnemyBase::CheckCombatTarget()
{
	//전투 가능 범위 밖이면 패트롤
	if(!InTargetRange(CombatTarget, CombatRadius))
	{
		ClearAttackTimer();
		LoseInterest();
		if(EnemyState != EEnemyState::EES_Engaged)
		{
			StartPatrolling();
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "LoseInterest");
	}
	//전투 가능 범위 안에서 감지하면 쫓아감
	else if(!InTargetRange(CombatTarget, AttackRadius))
	{
		if(EnemyState == EEnemyState::EES_Chasing) return;
		ClearAttackTimer();
		if(EnemyState != EEnemyState::EES_Engaged)
		{
			ChaseTarget();
		}
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Chase");
	}
	//공격 가능 범위 안이면 공격
	else if(CanAttack())
	{
		StartAttackTimer();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Attack");
	}
}

float AEnemyBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Status Component 에서 HP 줄이고 HP바에 표시하기
	CombatTarget = EventInstigator->GetPawn();
	ChaseTarget();
	return DamageAmount;
}

void AEnemyBase::HideHealthBar()
{
	if(HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}
}

void AEnemyBase::ShowHealthBar()
{
	if(HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(true);
	}
}

void AEnemyBase::LoseInterest()
{
	CombatTarget = nullptr;
	HideHealthBar();
}

void AEnemyBase::StartPatrolling()
{
	EnemyState = EEnemyState::EES_Patrolling;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	MoveToTarget(PatrolTarget);
}

void AEnemyBase::ChaseTarget()
{
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	MoveToTarget(CombatTarget);
}

void AEnemyBase::StartAttackTimer()
{
	EnemyState = EEnemyState::EES_Attacking;
	const float AttackTime = FMath::RandRange(AttackMin, AttackMax);
	GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemyBase::Attack, AttackTime, false);
}

void AEnemyBase::ClearAttackTimer()
{
	GetWorldTimerManager().ClearTimer(AttackTimer);
}

bool AEnemyBase::CanAttack()
{
	bool bCanAttack =
		InTargetRange(CombatTarget, AttackRadius) &&
		EnemyState != EEnemyState::EES_Attacking &&
		EnemyState != EEnemyState::EES_Dead;
	return bCanAttack;
}

void AEnemyBase::Attack()
{
	PlayAttackMontage();
}

void AEnemyBase::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, 2);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		case 2:
			SectionName = FName("Attack3");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AEnemyBase::DirectionalHitReact(const FVector& ImpactPoint)
{
	const FVector Forward = GetActorForwardVector();
	// Lower Impact Point to the Enemy's Actor Location Z
	const FVector ImpactLowered(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ToHit = (ImpactLowered - GetActorLocation()).GetSafeNormal();

	// Forward * ToHit = |Forward||ToHit| * cos(theta)
	// |Forward| = 1, |ToHit| = 1, so Forward * ToHit = cos(theta)
	const double CosTheta = FVector::DotProduct(Forward, ToHit);
	// Take the inverse cosine (arc-cosine) of cos(theta) to get theta
	double Theta = FMath::Acos(CosTheta);
	// convert from radians to degrees
	Theta = FMath::RadiansToDegrees(Theta);

	// if CrossProduct points down, Theta should be negative
	const FVector CrossProduct = FVector::CrossProduct(Forward, ToHit);
	if (CrossProduct.Z < 0)
	{
		Theta *= -1.f;
	}

	FName Section("FromBack");

	if (Theta >= -45.f && Theta < 45.f)
	{
		Section = FName("FromFront");
	}
	else if (Theta >= -135.f && Theta < -45.f)
	{
		Section = FName("FromLeft");
	}
	else if (Theta >= 45.f && Theta < 135.f)
	{
		Section = FName("FromRight");
	}

	//PlayHitReactMontage(Section);
}

void AEnemyBase::Die()
{
	// TODO: Play Death Montage
	// UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	// if (AnimInstance && DeathMontage)
	// {
	// 	AnimInstance->Montage_Play(DeathMontage);
	//
	// 	const int32 Selection = FMath::RandRange(0, 5);
	// 	FName SectionName = FName();
	// 	switch (Selection)
	// 	{
	// 	case 0:
	// 		SectionName = FName("Death1");
	// 		DeathPose = EDeathPose::EDP_Death1;
	// 		break;
	// 	case 1:
	// 		SectionName = FName("Death2");
	// 		DeathPose = EDeathPose::EDP_Death2;
	// 		break;
	// 	case 2:
	// 		SectionName = FName("Death3");
	// 		DeathPose = EDeathPose::EDP_Death3;
	// 		break;
	// 	case 3:
	// 		SectionName = FName("Death4");
	// 		DeathPose = EDeathPose::EDP_Death4;
	// 		break;
	// 	case 4:
	// 		SectionName = FName("Death5");
	// 		DeathPose = EDeathPose::EDP_Death5;
	// 		break;
	// 	case 5:
	// 		SectionName = FName("Death6");
	// 		DeathPose = EDeathPose::EDP_Death6;
	// 		break;
	// 	default:
	// 		break;
	// 	}
	//
	// 	AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	// }
	// if (HealthBarWidget)
	// {
	// 	HealthBarWidget->SetVisibility(false);
	// }
	// GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	// SetLifeSpan(3.f);
}

// void AEnemyBase::GetHit_Implementation(const FVector& ImpactPoint)
// {
// 	//DRAW_SPHERE_COLOR(ImpactPoint, FColor::Orange);
// 	if (HealthBarWidget)
// 	{
// 		HealthBarWidget->SetVisibility(true);
// 	}
// 	if (Attributes && Attributes->IsAlive())
// 	{
// 		DirectionalHitReact(ImpactPoint);
// 	}
// 	else
// 	{
// 		Die();
// 	}
//
// 	if (HitSound)
// 	{
// 		UGameplayStatics::PlaySoundAtLocation(
// 			this,
// 			HitSound,
// 			ImpactPoint
// 		);
// 	}
// 	if (HitParticles && GetWorld())
// 	{
// 		UGameplayStatics::SpawnEmitterAtLocation(
// 			GetWorld(),
// 			HitParticles,
// 			ImpactPoint
// 		);
// 	}
// }