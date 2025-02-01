// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Common/CommonStruct.h"
#include "SkillTemplate.generated.h"

// 버튼이 클릭될 때 호출될 delegate
DECLARE_DELEGATE_RetVal_OneParam(bool, FOnSkillButtonClicked, int32);

// 스킬이 할당된 후 호출될 delegate, argument, return 없음
DECLARE_DELEGATE(FOnSkillAssigned);

/**
 * 
 */
UCLASS()
class PUCKING_API USkillTemplate : public UUserWidget
{
	GENERATED_BODY()
	
	// SkillButton
	UPROPERTY(meta = (BindWidget))
	class UButton* SkillButton;

	// SkillText
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillText;

	// Essence
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EssenceText;

	// 그래프 구조를 만들기 위해 필요한 변수, 연결된 다른 USkillTemplate을 저장하는 Set
	TSet<USkillTemplate*> LinkedSkills;

	// 스킬이 할당되었는지 확인하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	bool bIsAssigned;

	// 해당 스킬이 할당 가능한지 확인하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	bool bIsAssignable;

	// 스킬 할당을 위해 필요한 자원 량
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	int32 RequiredEssence;

	// Assigned color
	FLinearColor AssignedColor = FLinearColor(1.0f, 0.0f, 0.0f, 1.0f);
	// Assignable color
	FLinearColor AssignableColor = FLinearColor(0.0f, 0.0f, 1.0f, 1.0f);

	// Skill Data
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Skill", meta = (AllowPrivateAccess = "true"))
	FSkillData SkillData;

protected:
	virtual void NativeOnInitialized() override;

public:
	// 스킬 버튼이 클릭되었을 때 호출되는 함수
	UFUNCTION()
	void OnSkillButtonClickedEvent();
	// 이 스킬과 다른 스킬을 연결하는 함수
	void LinkSkill(USkillTemplate* Skill);
	// 해당 스킬이 할당 가능한지 확인하는 함수
	bool IsAssignable() const;
	// 스킬이 할당되었는지 확인하는 함수
	bool IsAssigned() const { return bIsAssigned; }
	// bIsAssignable 을 설정하는 함수
	void SetAssignable(bool bAssignable);

	// SkillButton 의 style 의 normal tint color 를 설정하는 함수
	void SetButtonNormalTintColor(const FLinearColor& Color);
	// SkillButton 의 style 의 pressed, hovered 의 tint color 를 설정하는 함수
	void SetButtonPressedAndHoveredTintColor(const FLinearColor& Color);


	// 스킬을 할당하는 함수
	void AssignSkill();

	// SkillText에 텍스트를 설정하는 함수
	void SetSkillText(const FText& Text);

	// Set RequiredEssence
	void SetRequiredEssence(int32 Essence);

	// Set Skill Data
	void SetSkillData(const FSkillData& Data);

	FOnSkillButtonClicked OnSkillButtonClicked;
	FOnSkillAssigned OnSkillAssigned;
};
