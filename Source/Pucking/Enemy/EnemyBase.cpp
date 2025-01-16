// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

#include "AIController.h"
#include "ActorComponent/CloseCombatComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"

#include "Character/PuckingCharacter.h"
#include "ActorComponent/EnemyStatusComponent.h"
#include "UI/Enemy/HealthBarComponent.h"

#include "World/EnemyObjectPool.h"

AEnemyBase::AEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");
	PawnSensingComp->SightRadius = 4000.f;
	PawnSensingComp->SetPeripheralVisionAngle(45.f);

	StatusComp = CreateDefaultSubobject<UEnemyStatusComponent>("StatusComp");
	CloseCombatComp = CreateDefaultSubobject<UCloseCombatComponent>(TEXT("CloseCombatComp"));
	
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>("HealthBarWidget");
	HealthBarWidget->SetupAttachment(GetRootComponent());
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidget->SetDrawSize(FVector2D(150.0f, 20.0f));

	bIsActive = false;
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(EnemyState == EEnemyState::EES_Dead) return;
	if(EnemyState == EEnemyState::EES_Hit) return;
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
	Tags.Add("Enemy");
	HealthBarWidget->SetWidgetClass(HealthBarClass);
	HideHealthBar();
	
	EnemyController = Cast<AAIController>(GetController());
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemyBase::PawnSeen);
	StartPatrolling();
}

void AEnemyBase::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolAcceptanceRadius))
	{
		PatrolTarget = ChoosePatrolTarget();
		const float WaitTime = FMath::RandRange(PatrolWaitMin, PatrolWaitMax);
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemyBase::StartPatrolling, WaitTime);
	}
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

void AEnemyBase::StopMovement(const float Time)
{
	EEnemyState SavedState = EnemyState;
	const float SavedSpeed = GetCharacterMovement()->MaxWalkSpeed;
	
	EnemyState = EEnemyState::EES_Hit;
	GetCharacterMovement()->Deactivate();
	
	FTimerHandle StopTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StopTimerHandle, [this, SavedSpeed, SavedState]()
	{
		GetCharacterMovement()->Activate();
		GetCharacterMovement()->MaxWalkSpeed = SavedSpeed;
		EnemyState = SavedState;
	}, Time, false);
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
		HideHealthBar();
		ClearAttackTimer();
		LoseInterest();
		if(EnemyState != EEnemyState::EES_Engaged)
		{
			StartPatrolling();
		}
	}
	//전투 가능 범위 안에서 감지하면 쫓아감
	else if(!InTargetRange(CombatTarget, AttackRadius))
	{
		ShowHealthBar();
		if(EnemyState == EEnemyState::EES_Chasing) return;
		ClearAttackTimer();
		if(EnemyState != EEnemyState::EES_Engaged)
		{
			ChaseTarget();
		}
	}
	//공격 가능 범위 안이면 공격
	else if(CanAttack())
	{
		StartAttackTimer();
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
		EnemyState != EEnemyState::EES_Dead &&
		EnemyState != EEnemyState::EES_Engaged;
	return bCanAttack;
}

void AEnemyBase::AttackEnd()
{
	CloseCombatComp->ClearIgnoreActors();
	EnemyState = EEnemyState::EES_NoState;
	CheckCombatTarget();
}

void AEnemyBase::Attack()
{
	EnemyState = EEnemyState::EES_Engaged;
	PlayAttackMontage();
}

void AEnemyBase::Revive()
{
	bIsActive = true;
	bIsDead = false;
	StatusComp->EnemyStatInit();
	//StatusComp->SetComponentTickEnabled(true);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	SetActorTickEnabled(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetActorHiddenInGame(false);
}

void AEnemyBase::Die()
{
	PlayDeathMontage();
	GetWorld()->GetTimerManager().SetTimer(DeathAnimHandle, this, &AEnemyBase::ReturnAfterDelay, DeathLifeSpan, false);


	
	//ReturnAfterDelay(DeathLifeSpan);
	//SetLifeSpan(DeathLifeSpan);
	//SetActorTickEnabled(false);

}

void AEnemyBase::GetHit(const FHitResult& HitResult, const float StaggerTime)
{
	//Set HP Widget
	HealthBarWidget->SetHealthPercent(StatusComp->RemainHP/StatusComp->CurMaxHP);
	ShowHealthBar();
	if (StatusComp->RemainHP > 0)
	{
		DirectionalHitReact(HitResult.ImpactPoint);
		StopMovement(StaggerTime);
		CombatTarget = GetWorld()->GetFirstPlayerController()->GetCharacter();
		ChaseTarget();
	}
	else
	{
		//Die();
		ReturnPool();
	}
	// if (HitSound)
	// {
	// 	UGameplayStatics::PlaySoundAtLocation(
	// 		this,
	// 		HitSound,
	// 		ImpactPoint
	// 	);
	// }
	// if (HitParticles && GetWorld())
	// {
	// 	UGameplayStatics::SpawnEmitterAtLocation(
	// 		GetWorld(),
	// 		HitParticles,
	// 		ImpactPoint
	// 	);
	// }
}

void AEnemyBase::OnCombatCompAttachment(UStaticMeshComponent* TargetMeshComp, USceneComponent* BoxTraceStart,
	USceneComponent* BoxTraceEnd)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	FAttachmentTransformRules TransformRules_Relative(EAttachmentRule::KeepRelative, true);
	if(CloseCombatComp && TargetMeshComp && BoxTraceStart && BoxTraceEnd)
	{
		CloseCombatComp->AttachToComponent(GetMesh(), TransformRules, "CloseCombatSocket");
		CloseCombatComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		TargetMeshComp->AttachToComponent(CloseCombatComp, TransformRules);
		BoxTraceStart->AttachToComponent(CloseCombatComp, TransformRules_Relative);
		BoxTraceEnd->AttachToComponent(CloseCombatComp, TransformRules_Relative);
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
	PlayMontageSection(TakeHitMontage, Section);
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

int32 AEnemyBase::PlayAttackMontage()
{
	return PlayRandomMontageSection(AttackMontage, AttackMontageSections);
}

int32 AEnemyBase::PlayDeathMontage()
{
	const int32 Selection = PlayRandomMontageSection(DeathMontage, DeathMontageSections);
	TEnumAsByte<EDeathPose> Pose(Selection);
	if(Pose < EDeathPose::EDP_MAX)
	{
		DeathPose = Pose;
	}
	return Selection;
}

void AEnemyBase::PlayMontageSection(UAnimMontage* Montage, const FName& SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && Montage)
	{
		AnimInstance->Montage_Play(Montage);
		AnimInstance->Montage_JumpToSection(SectionName, Montage);
	}
}

int32 AEnemyBase::PlayRandomMontageSection(UAnimMontage* Montage, const TArray<FName>& SectionNames)
{
	if(SectionNames.Num() <= 0) return -1;
	const int32 MaxSectionIndex = SectionNames.Num() -1;
	const int32 Selection = FMath::RandRange(0, MaxSectionIndex);
	PlayMontageSection(Montage, SectionNames[Selection]);
	return Selection;
}




/// <summary>
/// Object Pool
/// </summary>
void AEnemyBase::Activate()
{
	bIsActive = true;
}

void AEnemyBase::Deactivate()
{
	bIsActive = false;
}

void AEnemyBase::Initialize(FVector SpawnLocation)
{
	SetActorLocation(SpawnLocation);
	Revive();
}

void AEnemyBase::ReturnPool()
{
	

	if (AEnemyObjectPool* Pool = GetWorld()->SpawnActor<AEnemyObjectPool>())
	{
		Pool->ReturnEnemy(this);
	}
}

void AEnemyBase::ReturnAfterDelay()
{
	bIsActive = false;
	bIsDead = true;
	ClearAttackTimer();
	HideHealthBar();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	SetActorTickEnabled(false);
	//StatusComp->SetComponentTickEnabled(false);
	SetActorHiddenInGame(true);
	SetActorLocation(FVector::ZeroVector);
}
