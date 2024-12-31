// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "AIController.h"
#include "Character/PuckingCharacter.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	PawnSensingComp->SightRadius = 4000.f;
	PawnSensingComp->SetPeripheralVisionAngle(45.f);
}

void AEnemyBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(EnemyState > EEnemyState::EES_Patrolling)
	{
		
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
	if (InTargetRange(PatrolTarget, PatrolRadius))
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
	MoveRequest.SetAcceptanceRadius(15.f);
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "PawnSeen");
	if(SeenPawn->ActorHasTag("Player"))
	{
		
	}
}

void AEnemyBase::CheckCombatTarget()
{
}

void AEnemyBase::DirectionalHitReact(const FVector& ImpactPoint)
{
	
}

void AEnemyBase::Die()
{
}