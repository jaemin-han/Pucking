// Fill out your copyright notice in the Description page of Project Settings.


#include "PuckPlayerState.h"

#include "ActorComponent/EquipComponent.h"
#include "ActorComponent/RifleActorComponent.h"
#include "ActorComponent/ShotGunActorComponent.h"
#include "Blueprint/UserWidget.h"
#include "UI/Skill/SkillTemplate.h"
#include "UI/Skill/SkillWidget.h"

class UEquipComponent;

void APuckPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// todo: debug 용 키세팅
	// 키보드 p 키를 누르면, 스킬 위젯을 화면에 띄운다.
	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::P))
	{
		if (SkillWidgetInstance && !SkillWidgetInstance->IsInViewport())
		{
			SkillWidgetInstance->AddToViewport();
			// input mode 를 game and UI 로 변경
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
			// mouse cursor 보이기
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
		}
	}
}

void APuckPlayerState::BeginPlay()
{
	Super::BeginPlay();

	// create skill widget instance
	SkillWidgetInstance = CreateWidget<USkillWidget>(GetWorld(), SkillWidgetClass);
	OnEssenceChanged.AddDynamic(SkillWidgetInstance, &USkillWidget::SetEssenceCount);

	BindFunctionToSkillWidget();

	SkillWidgetInstance->RootSkill->OnSkillButtonClickedEvent();
	SkillWidgetInstance->RifleSkill00->OnSkillButtonClickedEvent();
}

void APuckPlayerState::BindFunctionToSkillWidget()
{
	// player state 가 관리하는 player controller 가져오기
	APlayerController* PlayerController = GetPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is null"));
		return;
	}
	// player controller 에서 Character 가져오기
	APawn* PlayerPawn = PlayerController->GetPawn();
	if (!PlayerPawn)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerPawn is null"));
		return;
	}
	// player pawn 에서 모든 컴포넌트 가져오기
	TArray<UActorComponent*> Components;
	PlayerPawn->GetComponents(Components);

	// Components 에서 RifleComponent 가져오기
	URifleActorComponent* RifleComponent = nullptr;
	for (UActorComponent* Component : Components)
	{
		RifleComponent = Cast<URifleActorComponent>(Component);
		if (RifleComponent)
		{
			break;
		}
	}

	if (!RifleComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("RifleComponent is null"));
		return;
	}

	// RifleComponent 와 RifleSkill00 의 FOnSkillAssigned 에 바인딩
	SkillWidgetInstance->RifleSkill10->OnSkillAssigned.BindUObject(RifleComponent, &URifleActorComponent::DecreaseSpreadRange);
	SkillWidgetInstance->RifleSkill11->OnSkillAssigned.BindUObject(RifleComponent, &URifleActorComponent::IncreaseMaxMagazine);
	SkillWidgetInstance->RifleSkill12->OnSkillAssigned.BindUObject(RifleComponent, &URifleActorComponent::SetRateReloadAnimMontage);

	// Components 에서 ShotgunComponent 가져오기
	UShotgunActorComponent* ShotgunComponent = nullptr;
	for (UActorComponent* Component : Components)
	{
		ShotgunComponent = Cast<UShotgunActorComponent>(Component);
		if (ShotgunComponent)
		{
			break;
		}
	}

	if (!ShotgunComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("ShotgunComponent is null"));
		return;
	}

	// ShotgunComponent 와 ShotgunSkill00 의 FOnSkillAssigned 에 바인딩
	SkillWidgetInstance->ShotgunSkill10->OnSkillAssigned.BindUObject(ShotgunComponent, &UShotgunActorComponent::SetShootInterval);
	SkillWidgetInstance->ShotgunSkill11->OnSkillAssigned.BindUObject(ShotgunComponent, &UShotgunActorComponent::IncreaseBulletNum);
	SkillWidgetInstance->ShotgunSkill12->OnSkillAssigned.BindUObject(ShotgunComponent, &UShotgunActorComponent::SetRateReloadAnimMontage);

	/*
	 *	Skill00 계열 바인딩
	 *	EquipComponent 와 바인딩
	 */

	// EquipComponent 가져오기
	UEquipComponent* EquipComponent = nullptr;
	for (UActorComponent* Component : Components)
	{
		EquipComponent = Cast<UEquipComponent>(Component);
		if (EquipComponent)
		{
			break;
		}
	}

	// Skill00 의 FOnSkillAssigned 에 Activated 함수 바인딩
	SkillWidgetInstance->RifleSkill00->OnSkillAssigned.BindUObject(EquipComponent, &UEquipComponent::SetRifleActivated);
	SkillWidgetInstance->ShotgunSkill00->OnSkillAssigned.BindUObject(EquipComponent, &UEquipComponent::SetShotgunActivated);
	SkillWidgetInstance->UltimateSkill00->OnSkillAssigned.BindUObject(EquipComponent, &UEquipComponent::SetWeaponTBD1Activated);
	SkillWidgetInstance->HammerSkill00->OnSkillAssigned.BindUObject(EquipComponent, &UEquipComponent::SetWeaponTBD2Activated);
}

bool APuckPlayerState::ConsumeEssence(const int32 ConsumeEssence)
{
	if (Essence < ConsumeEssence)
	{
		return false;
	}

	Essence -= ConsumeEssence;
	OnEssenceChanged.Broadcast(Essence);
	return true;
}

APuckPlayerState::APuckPlayerState()
{
	// tick true
	PrimaryActorTick.bCanEverTick = true;
	Essence = 0;
}
