// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API USkillWidget : public UUserWidget
{
	GENERATED_BODY()

#pragma region Skill
public:
	// RootSkill
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* RootSkill;

	// RifleSkill00
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* RifleSkill00;

	// RifleSkill10
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* RifleSkill10;

	// RifleSkill11
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* RifleSkill11;

	// RifleSkill12
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* RifleSkill12;

	// ShotgunSkill00
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* ShotgunSkill00;

	// ShotgunSkill10
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* ShotgunSkill10;

	// ShotgunSkill11
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* ShotgunSkill11;

	// ShotgunSkill12
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* ShotgunSkill12;

	// UltimateSkill00
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* UltimateSkill00;

	// UltimateSkill10
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* UltimateSkill10;

	// UltimateSkill11
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* UltimateSkill11;

	// UltimateSkill12
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* UltimateSkill12;

	// HammerSkill00
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* HammerSkill00;

	// HammerSkill10
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* HammerSkill10;

	// HammerSkill11
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* HammerSkill11;

	// HammerSkill12
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* HammerSkill12;
#pragma endregion

private:
	// EssenceCount
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EssenceCount;

	// TitleCloseBtn
	UPROPERTY(meta = (BindWidget))
	class UButton* TitleCloseBtn;

protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION()
	void OnTitleCloseBtnClicked();
public:
	// set EssenceCount
	UFUNCTION()
	void SetEssenceCount(const int32 Essence);	
	
};
