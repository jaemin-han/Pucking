// Fill out your copyright notice in the Description page of Project Settings.


#include "PuckPlayerState.h"

#include "Blueprint/UserWidget.h"
#include "UI/Skill/SkillWidget.h"

void APuckPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// todo: debug 용 키세팅
	// 키보드 p 키를 누르면, 스킬 위젯을 화면에 띄운다.
	if (GetWorld()->GetFirstPlayerController()->IsInputKeyDown(EKeys::P))
	{
		if (SkillWidgetInstance && SkillWidgetInstance->IsInViewport())
		{
			SkillWidgetInstance->RemoveFromParent();
			// input mode 를 game only 로 변경
			GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
			// mouse cursor 숨기기
			GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
		}
		else if (SkillWidgetInstance && !SkillWidgetInstance->IsInViewport())
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
