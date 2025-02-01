// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillTemplate.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"

void USkillTemplate::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	SkillButton->OnClicked.AddDynamic(this, &USkillTemplate::OnSkillButtonClickedEvent);
}

void USkillTemplate::LinkSkill(USkillTemplate* Skill)
{
	LinkedSkills.Add(Skill);
	Skill->LinkedSkills.Add(this);
}

bool USkillTemplate::IsAssignable() const
{
	// 해당 스킬이 할당되었다면, 할당 불가능
	if (IsAssigned())
		return false;
	return bIsAssignable;
}

void USkillTemplate::SetAssignable(bool bAssignable)
{
	bIsAssignable = bAssignable;
	// todo: 버튼 활성화 및 시각적 효과
	if (bAssignable && !IsAssigned())
	{
		SetButtonNormalTintColor(AssignableColor);
	}
}

void USkillTemplate::AssignSkill()
{
	bIsAssigned = true;
	if (!OnSkillAssigned.ExecuteIfBound())
	{
		UE_LOG(LogTemp, Warning, TEXT("OnSkillAssigned is not bound"));
	}
	// 다른 연결된 스킬들의 bIsAssignable 을 true 로 설정
	for (auto* LinkedSkill : LinkedSkills)
	{
		LinkedSkill->SetAssignable(true);
	}
	SetButtonNormalTintColor(AssignedColor);
	SetButtonPressedAndHoveredTintColor(AssignedColor);
}

void USkillTemplate::OnSkillButtonClickedEvent()
{
	// IsAssignable 이 false 이면 할당 불가능
	if (!IsAssignable())
	{
		UE_LOG(LogTemp, Warning, TEXT("Skill is not assignable"));
		return;
	}

	// delegate 호출
	// Essence 가 충분해 스킬이 할당되었음
	if (OnSkillButtonClicked.Execute(RequiredEssence))
	{
		AssignSkill();
	}
	// Essence 가 부족해 스킬이 할당되지 않음
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Essence is not enough"));
	}
}

void USkillTemplate::SetSkillText(const FText& Text)
{
	SkillText->SetText(Text);
}

void USkillTemplate::SetRequiredEssence(int32 Essence)
{
	RequiredEssence = Essence;
	EssenceText->SetText(FText::FromString(FString::FromInt(Essence)));
}

void USkillTemplate::SetSkillData(const FSkillData& Data)
{
	SkillData = Data;
	SetSkillText(Data.WidgetText);
	SetRequiredEssence(Data.RequiredEssence);
}

void USkillTemplate::SetButtonNormalTintColor(const FLinearColor& Color)
{
	if (!SkillButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("SkillButton is null"));
		return;
	}

	// 기존 스타일 가져오기
	FButtonStyle ButtonStyle = SkillButton->GetStyle();

	// Normal 상태의 Tint Color 설정
	ButtonStyle.Normal.TintColor = FSlateColor(Color);

	// 변경된 스타일 적용
	SkillButton->SetStyle(ButtonStyle);
}

void USkillTemplate::SetButtonPressedAndHoveredTintColor(const FLinearColor& Color)
{
	if (!SkillButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("SkillButton is null"));
		return;
	}

	FButtonStyle ButtonStyle = SkillButton->GetStyle();
	ButtonStyle.Hovered.TintColor = FSlateColor(Color);
	ButtonStyle.Pressed.TintColor = FSlateColor(Color);

	SkillButton->SetStyle(ButtonStyle);
}