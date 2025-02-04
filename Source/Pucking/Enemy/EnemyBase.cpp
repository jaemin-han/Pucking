// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

#include "AIController.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ActorComponent/CloseCombatComponent.h"
#include "ActorComponent/DropItemComponent.h"
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
	HealthBarWidget->SetupAttachment(GetMesh());
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidget->SetDrawSize(FVector2D(150.0f, 20.0f));

	bIsActive = false;
}

void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(EnemyState == EEnemyState::EES_BlackHole) return;
	if(EnemyState == EEnemyState::EES_Dead || EnemyState == EEnemyState::EES_Hit) return;

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
	PuckGameInstance = Cast<UPuckGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	EnemyController = Cast<AAIController>(GetController());
	if (!EnemyController)
	{
		EnemyController = GetWorld()->SpawnActor<AAIController>();
		if (EnemyController)
		{
			EnemyController->Possess(this);
		}
	}
	
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemyBase::PawnSeen);
	PatrolTarget = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void AEnemyBase::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolAcceptanceRadius))
	{
		//PatrolTarget = ChoosePatrolTarget();
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
		EnemyState != EEnemyState::EES_BlackHole &&
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
		if(EnemyState != EEnemyState::EES_Engaged && EnemyState != EEnemyState::EES_BlackHole) 
		{
			StartPatrolling();
		}
	}
	//전투 가능 범위 안에서 감지하면 쫓아감
	else if(!InTargetRange(CombatTarget, AttackRadius))
	{
		if(EnemyState == EEnemyState::EES_BlackHole) return;
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
		EnemyState != EEnemyState::EES_BlackHole &&
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
	SetActorTickEnabled(true);
	bIsActive = true;
	bIsDead = false;
	EnemyState = EEnemyState::EES_NoState;
	StartPatrolling();
	EnemyBaseStatusInit();
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	SetActorHiddenInGame(false);
	//동작 재생(나중에 다른방법 있으면 체크해봐야할듯)
	HealthBarWidget->SetTypeImage(StatusComp->CommonDamageType);
	GetCharacterMovement()->SetMovementMode(MOVE_Walking);
}

void AEnemyBase::Die()
{
	//살아있는 판정일 때
	if (bIsDead == false)
	{
		DropItems();
		PlayDeathMontage();
	}
	//쉬는시간이 아니고, 살아있는 상태인 적을 죽일 때만 점수 카운트
	if ((PuckGameInstance->bIsHalf == false) && (bIsDead == false))
	{
		PuckGameInstance->DoKillCount();
	}
	ClearAttackTimer();
	GetWorldTimerManager().ClearTimer(BlackHoleTimer);
	bIsBeingSucked = false;
	GetWorld()->GetTimerManager().SetTimer(DeathAnimHandle, this, &AEnemyBase::ReturnAfterDelay, DeathLifeSpan, false);

	EnemyState = EEnemyState::EES_Dead;
	//PlayDeathMontage();
	SetActorTickEnabled(false);
	HideHealthBar();
	GetCharacterMovement()->bOrientRotationToMovement = false;
	//동작 멈춤(나중에 다른방법 있으면 체크해봐야할듯)
	//GetCharacterMovement()->DisableMovement();
	
	//죽음 판정
	bIsDead = true;
	//ReturnAfterDelay(DeathLifeSpan);
	//SetLifeSpan(DeathLifeSpan);
	//SetActorTickEnabled(false);
}

void AEnemyBase::HitByExplosion(FVector ExplosionLocation)
{
	EnemyController->StopMovement();
	ClearAttackTimer();
	LoseInterest();
	EnemyState = EEnemyState::EES_BlackHole;
}

void AEnemyBase::HitByBlackHole(FVector BlackHoleLocation)
{
	EnemyController->StopMovement();
	ClearAttackTimer();
	
	bIsBeingSucked = true;
	BlackHoleTarget = BlackHoleLocation;
	//ClearAttackTimer();
	LoseInterest();
	SuckedByBlackHole();
	GetWorld()->GetTimerManager().SetTimer(BlackHoleTimer,this, &AEnemyBase::SuckedByBlackHole, 0.3f, true);
}

void AEnemyBase::SuckedByBlackHole()
{
	if(EnemyState == EEnemyState::EES_Dead) return;
	if(bIsBeingSucked)
	{
		EnemyState = EEnemyState::EES_BlackHole;
		GetCharacterMovement()->MaxWalkSpeed = BlackHoleSpeed;
		EnemyController->MoveToLocation(BlackHoleTarget);
	}
}

void AEnemyBase::SetHealthShieldBar()
{
	//Set HP Widget
	HealthBarWidget->SetHealthPercent(StatusComp->RemainHP/StatusComp->CurMaxHP);
	HealthBarWidget->SetShieldPercent(StatusComp->RemainShield/StatusComp->CurMaxShield);
}

void AEnemyBase::GetHit(const FHitResult& HitResult, const float StaggerTime)
{
	SetHealthShieldBar();
	ShowHealthBar();
	if (StatusComp->RemainHP > 0)
	{
		if(ScreamSound)PlaySound(ScreamSound, HitResult.ImpactPoint);
		DirectionalHitReact(HitResult.ImpactPoint);
		StopMovement(StaggerTime);
		CombatTarget = GetWorld()->GetFirstPlayerController()->GetCharacter();
		ChaseTarget();
	}
	else
	{
		if(DeathSound)PlaySound(DeathSound, HitResult.ImpactPoint);
		//Die();
		ReturnPool();
		GetMesh()->SetCollisionResponseToChannel(ECC_GameTraceChannel3, ECR_Ignore);
	}
	const int32	HitSoundIndex = HitSounds.Num() -1;

	const int32 Selection = FMath::RandRange(0, HitSoundIndex);
	if (HitSounds[Selection])
	{
		PlaySound(HitSounds[Selection], HitResult.ImpactPoint);
	}
	if (BloodEffects[Selection])
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(),
		BloodEffects[Selection],
		HitResult.ImpactPoint,
		FRotator(0, 0, 0),
		FVector(1.f)
		);
	}
}

void AEnemyBase::OnCombatCompAttachment(UStaticMeshComponent* TargetMeshComp, USceneComponent* BoxTraceStart,
	USceneComponent* BoxTraceEnd)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	FAttachmentTransformRules TransformRules_Relative(EAttachmentRule::KeepRelative, true);
	if(CloseCombatComp && TargetMeshComp && BoxTraceStart && BoxTraceEnd)
	{
		TargetMeshComp->AttachToComponent(GetMesh(), TransformRules, "CloseCombatSocket");
		BoxTraceStart->AttachToComponent(TargetMeshComp, TransformRules_Relative);
		BoxTraceEnd->AttachToComponent(TargetMeshComp, TransformRules_Relative);
		CloseCombatComp->AttachToComponent(TargetMeshComp, TransformRules_Relative);
		CloseCombatComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
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

void AEnemyBase::PlaySound(USoundBase* Sound, const FVector& Location)
{
	UGameplayStatics::PlaySoundAtLocation(
	this,
	Sound,
	Location);
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
	AEnemyObjectPool* Pool = Cast<AEnemyObjectPool>(UGameplayStatics::GetActorOfClass(GetWorld(), AEnemyObjectPool::StaticClass()));
	if (Pool)
	{
		Pool->ReturnEnemy(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("NO ObjectEnemyPool in the World"));
	}
}

void AEnemyBase::ReturnAfterDelay()
{
	bIsActive = false;
	//bIsDead = true;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorHiddenInGame(true);
	SetActorLocation(FVector::ZeroVector);
}

void AEnemyBase::EnemyBaseStatusInit()
{
	StatusComp->EnemyStatInit();
}

void AEnemyBase::DropItems()
{
	// DropItemComponent 를 가져온다
	UDropItemComponent* DropItemComponent = FindComponentByClass<UDropItemComponent>();
	if (DropItemComponent)
	{
		// DropItemComponent 의 ItemTier, ItemRarityMultiplier, DropRateMultiplier 를 출력
		// UE_LOG(LogTemp, Warning, TEXT("ItemTier : %d"), DropItemComponent->ItemTier);
		// UE_LOG(LogTemp, Warning, TEXT("ItemRarityMultiplier : %f"), DropItemComponent->ItemRarityMultiplier);
		// UE_LOG(LogTemp, Warning, TEXT("DropRateMultiplier : %f"), DropItemComponent->DropRateMultiplier);
		DropItemComponent->DropItem();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DropItemComponent is not found"));
	}
}
