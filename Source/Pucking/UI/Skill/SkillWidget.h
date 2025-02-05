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
	// 각 무기군 별로 스킬이 존재한다
	// 무기군은 Rifle, Shotgun, Ultimate, Hammer
	// 무기 별로 무기를 장비하기 위한 스킬 00
	// 각 무기의 특성을 강화시키는 10 ~ 12, 20 ~ 22, 30 ~ 32 총 9개의 스킬이 존재한다.

	// Rifle
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* RifleSkill00;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* RifleSkill10;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* RifleSkill11;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* RifleSkill12;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* RifleSkill20;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* RifleSkill21;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* RifleSkill22;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* RifleSkill30;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* RifleSkill31;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* RifleSkill32;

	// Shotgun
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* ShotgunSkill00;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* ShotgunSkill10;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* ShotgunSkill11;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* ShotgunSkill12;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* ShotgunSkill20;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* ShotgunSkill21;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* ShotgunSkill22;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* ShotgunSkill30;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* ShotgunSkill31;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* ShotgunSkill32;

	// Ultimate
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* UltimateSkill00;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* UltimateSkill10;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* UltimateSkill11;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* UltimateSkill12;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* UltimateSkill20;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* UltimateSkill21;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* UltimateSkill22;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* UltimateSkill30;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* UltimateSkill31;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* UltimateSkill32;

	// Hammer
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* HammerSkill00;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* HammerSkill10;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* HammerSkill11;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* HammerSkill12;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* HammerSkill20;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* HammerSkill21;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* HammerSkill22;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* HammerSkill30;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* HammerSkill31;
	UPROPERTY(meta = (BindWidget))
	class USkillTemplate* HammerSkill32;
#pragma endregion

private:
	// EssenceCount
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* EssenceCount;

	// TitleCloseBtn
	UPROPERTY(meta = (BindWidget))
	class UButton* TitleCloseBtn;

	// CanvasPanel_Skills
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* CanvasPanel_Skills;

	// Slider_Scroll
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	UUserWidget* Slider_Scroll;

	// 현재 Y Trasnform 값, blueprint 에서 사용하기 위해 UPROPERTY 로 선언
	UPROPERTY(BlueprintReadWrite, Category = "SkillWidget", meta = (AllowPrivateAccess = "true"))
	float CurrentYTransform = 0.0f;

	// clamp range
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillWidget", meta = (AllowPrivateAccess = "true"))
	float YRange = 200.0f;

	// Skill Data Table
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SkillWidget", meta = (AllowPrivateAccess = "true"))
	class UDataTable* SkillDataTable;
	
protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	// set EssenceCount
	UFUNCTION()
	void SetEssenceCount(const int32 Essence);

	// CanvasPanel_Skills 의 모든 자식의 transform y 값을 을 조정하는 함수
	UFUNCTION()
	void SetCanvasPanel_SkillsPositionY(bool bIsUp);

	//
	UFUNCTION(BlueprintCallable)
	void SetAllChildYTransform(float NewYTransform);


	// Get CurrentYTransform
	float GetCurrentYTransform() const { return CurrentYTransform; }

	// Set CurrentYTransform
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SkillWidget")
	void SetCurrentYTransform(float NewYTransform);
};