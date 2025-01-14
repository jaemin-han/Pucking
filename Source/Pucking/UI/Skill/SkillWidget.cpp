// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillWidget.h"

#include "SkillTemplate.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "World/PuckPlayerState.h"

void USkillWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// 그래프 구조 만들기
	RootSkill->LinkSkill(RifleSkill00);
	RootSkill->LinkSkill(ShotgunSkill00);
	RootSkill->LinkSkill(UltimateSkill00);
	RootSkill->LinkSkill(HammerSkill00);

	RifleSkill00->LinkSkill(RifleSkill10);
	RifleSkill00->LinkSkill(RifleSkill11);
	RifleSkill00->LinkSkill(RifleSkill12);

	ShotgunSkill00->LinkSkill(ShotgunSkill10);
	ShotgunSkill00->LinkSkill(ShotgunSkill11);
	ShotgunSkill00->LinkSkill(ShotgunSkill12);

	UltimateSkill00->LinkSkill(UltimateSkill10);
	UltimateSkill00->LinkSkill(UltimateSkill11);
	UltimateSkill00->LinkSkill(UltimateSkill12);

	HammerSkill00->LinkSkill(HammerSkill10);
	HammerSkill00->LinkSkill(HammerSkill11);
	HammerSkill00->LinkSkill(HammerSkill12);

	// Text 초기화하기
	RootSkill->SetSkillText(FText::FromString(TEXT("RootSkill")));
	RifleSkill00->SetSkillText(FText::FromString(TEXT("RifleSkill00")));
	RifleSkill10->SetSkillText(FText::FromString(TEXT("RifleSkill10")));
	RifleSkill11->SetSkillText(FText::FromString(TEXT("RifleSkill11")));
	RifleSkill12->SetSkillText(FText::FromString(TEXT("RifleSkill12")));
	ShotgunSkill00->SetSkillText(FText::FromString(TEXT("ShotgunSkill00")));
	ShotgunSkill10->SetSkillText(FText::FromString(TEXT("ShotgunSkill10")));
	ShotgunSkill11->SetSkillText(FText::FromString(TEXT("ShotgunSkill11")));
	ShotgunSkill12->SetSkillText(FText::FromString(TEXT("ShotgunSkill12")));
	UltimateSkill00->SetSkillText(FText::FromString(TEXT("UltimateSkill00")));
	UltimateSkill10->SetSkillText(FText::FromString(TEXT("UltimateSkill10")));
	UltimateSkill11->SetSkillText(FText::FromString(TEXT("UltimateSkill11")));
	UltimateSkill12->SetSkillText(FText::FromString(TEXT("UltimateSkill12")));
	HammerSkill00->SetSkillText(FText::FromString(TEXT("HammerSkill00")));
	HammerSkill10->SetSkillText(FText::FromString(TEXT("HammerSkill10")));
	HammerSkill11->SetSkillText(FText::FromString(TEXT("HammerSkill11")));
	HammerSkill12->SetSkillText(FText::FromString(TEXT("HammerSkill12")));

	// 필요한 Essence 초기화하기
	RootSkill->SetRequiredEssence(0);
	RifleSkill00->SetRequiredEssence(0);
	RifleSkill10->SetRequiredEssence(20);
	RifleSkill11->SetRequiredEssence(20);
	RifleSkill12->SetRequiredEssence(20);
	ShotgunSkill00->SetRequiredEssence(10);
	ShotgunSkill10->SetRequiredEssence(20);
	ShotgunSkill11->SetRequiredEssence(20);
	ShotgunSkill12->SetRequiredEssence(20);
	UltimateSkill00->SetRequiredEssence(10);
	UltimateSkill10->SetRequiredEssence(20);
	UltimateSkill11->SetRequiredEssence(20);
	UltimateSkill12->SetRequiredEssence(20);
	HammerSkill00->SetRequiredEssence(10);
	HammerSkill10->SetRequiredEssence(20);
	HammerSkill11->SetRequiredEssence(20);
	HammerSkill12->SetRequiredEssence(20);

	// APuckPlayerState 가져오기
	APuckPlayerState* PlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<APuckPlayerState>();
	if (!PlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerState is null"));
		return;
	}

	// 모든 SkillTemplate의 OnSkillButtonClicked에 바인딩
	TArray<USkillTemplate*> SkillTemplates = {
		RootSkill, RifleSkill00, RifleSkill10, RifleSkill11, RifleSkill12,
		ShotgunSkill00, ShotgunSkill10, ShotgunSkill11, ShotgunSkill12,
		UltimateSkill00, UltimateSkill10, UltimateSkill11, UltimateSkill12,
		HammerSkill00, HammerSkill10, HammerSkill11, HammerSkill12
	};

	for (USkillTemplate* Skill : SkillTemplates)
	{
		if (Skill)
		{
			Skill->OnSkillButtonClicked.BindUObject(PlayerState, &APuckPlayerState::ConsumeEssence);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Skill is null"));
		}
	}

	// root skill 의 bIsAssignable 을 true 로 설정
	RootSkill->SetAssignable(true);

	// EssenceCount 초기화하기
	TitleCloseBtn->OnClicked.AddDynamic(this, &USkillWidget::OnTitleCloseBtnClicked);
}

void USkillWidget::OnTitleCloseBtnClicked()
{
	RemoveFromParent();
	// input mode 를 game only 로 변경
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
	// mouse cursor 숨기기
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
}

void USkillWidget::SetEssenceCount(const int32 Essence)
{
	EssenceCount->SetText(FText::FromString(FString::FromInt(Essence)));
}
