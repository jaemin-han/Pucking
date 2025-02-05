// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/PlayerStatusComponent.h"
#include "ActorComponent/EnemyStatusComponent.h"
#include "Character/PuckingCharacter.h"
#include "GameFramework/Character.h"
#include "World/PuckGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/SubUI/SubHPShieldUI.h"
#include "Components/CapsuleComponent.h"
#include "UI/HUD/MainHUD.h"

void UPlayerStatusComponent::BeginPlay()
{
	Super::BeginPlay();
	PuckGameInstance = Cast<UPuckGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	PuckCharacter = Cast<APuckingCharacter>(Owner);
	EquipComp = Owner->FindComponentByClass<UEquipComponent>();
	EquipComp->OnStatusComponentChanged.AddDynamic(this, &UStatusComponent::ApplyOption);
	PuckGameInstance->OnGameClear.AddDynamic(this, &UPlayerStatusComponent::AllWidgetClear);
	//SubHPShieldUI Change
	OnCharacterHPShieldChanged.Broadcast();
	

	UPuckGameInstance* GameInstance = Cast<UPuckGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameInstance->LevelCheck();
		UE_LOG(LogTemp, Warning, TEXT("PuckGameInstance IsValid"))
	}
}

void UPlayerStatusComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	//GEngine->AddOnScreenDebugMessage(-1, 0.005f, FColor::Green, FString::Printf(TEXT("Damage : %f"), CurDamage));
	//GEngine->AddOnScreenDebugMessage(-1, 0.005f, FColor::Green, FString::Printf(TEXT("Critical_Chance : %f"), CurCriticalChance));
	//GEngine->AddOnScreenDebugMessage(-1, 0.005f, FColor::Green, FString::Printf(TEXT("Critical_Multipier : %f"), CurCriticalMultipier));
	//GEngine->AddOnScreenDebugMessage(-1, 0.005f, FColor::Green, FString::Printf(TEXT("Physical_Penetration : %f"), CurPhysicalPenetration));
	//GEngine->AddOnScreenDebugMessage(-1, 0.005f, FColor::Green, FString::Printf(TEXT("Fire_Penetration : %f"), CurFirePenetration));
	//GEngine->AddOnScreenDebugMessage(-1, 0.005f, FColor::Green, FString::Printf(TEXT("Ice_Penetration : %f"), CurIcePenetration));
	//GEngine->AddOnScreenDebugMessage(-1, 0.005f, FColor::Green, FString::Printf(TEXT("StaggerValue : %f"), CurStaggerValue));
	//GEngine->AddOnScreenDebugMessage(-1, 0.005f, FColor::Yellow, FString::Printf(TEXT("DamageType : %s"), *EnumValueName));
	//GEngine->AddOnScreenDebugMessage(-1, 0.005f, FColor::Red, FString::Printf(TEXT("Owner Name : %s"), *Owner->GetName()));

	//DrawDebugString(GetWorld(), GetOwner()->GetActorLocation() - FVector(0, 0, 20), FString::Printf(TEXT("HP : %.1f"), RemainHP), 0, FColor::Red, 0.005f, false, 2.0f);

	//DrawDebugString(GetWorld(), GetOwner()->GetActorLocation(), FString::Printf(TEXT("SHIELD : %.1f"), RemainShield), 0, FColor::White, 0.005f, false, 2.0f);
	//캐릭터 위치 찾기
	//NiagaraComp->SetWorldLocation(GetOwner()->GetActorLocation());
	//NiagaraComp->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
}

//줄 데미지 계산
void UPlayerStatusComponent::DamageCalculation()
{
	switch (CommonDamageType)
	{
	case EDamageType::Physical:
		PenetrationType = CurPhysicalPenetration;
		break;
	case EDamageType::Fire:
		PenetrationType = CurFirePenetration;
		break;
	case EDamageType::Ice:
		PenetrationType = CurIcePenetration;
		break;
	default:
		break;
	}
	//PlayerStatus DamageCalculation
	float CriticalRange = FMath::FRandRange(0.0f, 100.0f);
	if (CurCriticalChance >= CriticalRange)
	{
		DamageAmount = CurDamage * CurCriticalMultipier;

	}
	else if (CurCriticalChance < CriticalRange)
	{
		DamageAmount = CurDamage;
	}
	StaggerAmount = CurStaggerValue;
	
}

//방어력 계산해서 받는 데미지 결정
void UPlayerStatusComponent::GetDamage(EDamageType GetDamageType, float GetdamageAmount, float Penetration, float GetStaggerValue, const FHitResult& _hitRes)
{

	RemainShield -= GetdamageAmount;
	//실드가 없는 상태면
	if (RemainShield <= 0)
	{
		//나이아가라 끄기
		//NiagaraComp->Deactivate();
		//NiagaraComp->SetVisibility(false);

		RemainShield = 0;
		//체력 처리로 이동
		switch (GetDamageType)
		{
		case EDamageType::Physical:
			DefenseAmount = CurPhysicalDefense - Penetration;
			if (DefenseAmount >= GetdamageAmount)
			{
				RemainHP -= 1;
			}
			else if (DefenseAmount < GetdamageAmount)
			{
				RemainHP = RemainHP - (GetdamageAmount - DefenseAmount);
			}
			break;
		case EDamageType::Fire:
			DefenseAmount = CurFireDefense - Penetration;
			if (DefenseAmount >= GetdamageAmount)
			{
				RemainHP -= 1;
			}
			else if (DefenseAmount < GetdamageAmount)
			{
				RemainHP = RemainHP - (GetdamageAmount - DefenseAmount);
			}
			break;
		case EDamageType::Ice:
			DefenseAmount = CurIceDefense - Penetration;
			if (DefenseAmount >= GetdamageAmount)
			{
				RemainHP -= 1;
			}
			else if (DefenseAmount < GetdamageAmount)
			{
				RemainHP = RemainHP - (GetdamageAmount - DefenseAmount);
			}
			break;
		default:
			break;
		}

		
		if (RemainHP <= 0)
		{
			Die();
		}
	}
	//SubHPShieldUI Change
	//UE_LOG(LogTemp, Error, TEXT("Is Deletagted : %d"), OnCharacterHPShieldChanged.IsBound());
	OnCharacterHPShieldChanged.Broadcast();

	APuckingCharacter* OwnerEnemy = Cast<APuckingCharacter>(GetOwner());
	if(OwnerEnemy)
	{
		OwnerEnemy->GetHit(_hitRes);
	}
	
	//피해를 받으면 회복중이던 타이머 멈춤(삭제)
	GetOwner()->GetWorld()->GetTimerManager().ClearTimer(RecoverySpeedTimer);

	//데미지 받고 3초 후 실드 회복 시작
	GetOwner()->GetWorld()->GetTimerManager().SetTimer(RecoveryDelayTimer, this, &UStatusComponent::ShieldRecovery, 3.0f, false);
	
}

//맞은 Actor를 받아서 데미지 처리 실행
void UPlayerStatusComponent::DamageProcessing(AActor* hitActor, const FHitResult& _hitRes)
{
	//맞은 타겟에 EnemyStatusComponent가 있으면
	if (IsValid(hitActor))
	{
		TargetEnemyComp = hitActor->FindComponentByClass<UEnemyStatusComponent>();
		if (TargetEnemyComp)
		{
			//내가 줄 데미지 계산하고
			DamageCalculation();
			//맞은 타겟의 EnemyStatusComponent의 GetDamage를 실행
			TargetEnemyComp->GetDamage(CommonDamageType, DamageAmount, PenetrationType, StaggerAmount, _hitRes);
		}
		else return;

	}
	else return;
}

void UPlayerStatusComponent::Die()
{
	//레그돌
	Owner->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Owner->GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

	Owner->GetMesh()->SetSimulatePhysics(true);
	Owner->GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	Owner->GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	APlayerController* PC = Cast<APlayerController>(Owner->GetController());
	if (PC)
	{
		PC->DisableInput(PC);
	}
	//모든위젯 끄고, 게임오버 위젯 활성화
	AllWidgetClear();
	PuckGameInstance->ShowGameOverWidget();
	//Owner->Destroy();
}

void UPlayerStatusComponent::EatHealingPack(float GetHealAmount)
{
	RemainHP += GetHealAmount;
	if (RemainHP > CurMaxHP)
	{
		RemainHP = CurMaxHP;
		UE_LOG(LogTemp, Warning, TEXT("Full HP"));
	}
	OnCharacterHPShieldChanged.Broadcast();
}

void UPlayerStatusComponent::AllWidgetClear()
{
	TSet<UUserWidget*> FoundWidget;
	for (UUserWidget* Widget : PuckCharacter->WidgetSet)
	{
		Widget->RemoveFromParent();
	}
	PuckCharacter->MainHUD->RemoveFromParent();
}
