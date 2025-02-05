// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillWidget.h"

#include "SkillTemplate.h"
#include "Character/PuckingCharacter.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "World/PuckPlayerState.h"

void USkillWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// TitleCloseBtn에 OnClicked 바인딩
	// APuckingCharacter 가져오기
	APuckingCharacter* PuckingCharacter = Cast<APuckingCharacter>(GetOwningPlayerPawn());
	if (!PuckingCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("PuckingCharacter is null"));
		return;
	}
	TitleCloseBtn->OnClicked.AddDynamic(PuckingCharacter, &APuckingCharacter::SkillWidgetOnOff);


	RifleSkill00->LinkSkill(RifleSkill10);
	RifleSkill10->LinkSkill(RifleSkill11);
	RifleSkill11->LinkSkill(RifleSkill12);

	RifleSkill00->LinkSkill(RifleSkill20);
	RifleSkill20->LinkSkill(RifleSkill21);
	RifleSkill21->LinkSkill(RifleSkill22);

	RifleSkill00->LinkSkill(RifleSkill30);
	RifleSkill30->LinkSkill(RifleSkill31);
	RifleSkill31->LinkSkill(RifleSkill32);

	ShotgunSkill00->LinkSkill(ShotgunSkill10);
	ShotgunSkill10->LinkSkill(ShotgunSkill11);
	ShotgunSkill11->LinkSkill(ShotgunSkill12);

	ShotgunSkill00->LinkSkill(ShotgunSkill20);
	ShotgunSkill20->LinkSkill(ShotgunSkill21);
	ShotgunSkill21->LinkSkill(ShotgunSkill22);

	ShotgunSkill00->LinkSkill(ShotgunSkill30);
	ShotgunSkill30->LinkSkill(ShotgunSkill31);
	ShotgunSkill31->LinkSkill(ShotgunSkill32);

	UltimateSkill00->LinkSkill(UltimateSkill10);
	UltimateSkill10->LinkSkill(UltimateSkill11);
	UltimateSkill11->LinkSkill(UltimateSkill12);

	UltimateSkill00->LinkSkill(UltimateSkill20);
	UltimateSkill20->LinkSkill(UltimateSkill21);
	UltimateSkill21->LinkSkill(UltimateSkill22);

	UltimateSkill00->LinkSkill(UltimateSkill30);
	UltimateSkill30->LinkSkill(UltimateSkill31);
	UltimateSkill31->LinkSkill(UltimateSkill32);

	HammerSkill00->LinkSkill(HammerSkill10);
	HammerSkill10->LinkSkill(HammerSkill11);
	HammerSkill11->LinkSkill(HammerSkill12);

	HammerSkill00->LinkSkill(HammerSkill20);
	HammerSkill20->LinkSkill(HammerSkill21);
	HammerSkill21->LinkSkill(HammerSkill22);

	HammerSkill00->LinkSkill(HammerSkill30);
	HammerSkill30->LinkSkill(HammerSkill31);
	HammerSkill31->LinkSkill(HammerSkill32);

	/*
		// Rifle Skills
		RifleSkill00->SetSkillText(FText::FromString(TEXT("SetRifleActivated")));
		RifleSkill00->SetRequiredEssence(0);
	
		RifleSkill10->SetSkillText(FText::FromString(TEXT("SetShootInterval1")));
		RifleSkill10->SetRequiredEssence(10);
		RifleSkill11->SetSkillText(FText::FromString(TEXT("SetShootInterval2")));
		RifleSkill11->SetRequiredEssence(20);
		RifleSkill12->SetSkillText(FText::FromString(TEXT("SetShootInterval3")));
		RifleSkill12->SetRequiredEssence(30);
	
		RifleSkill20->SetSkillText(FText::FromString(TEXT("IncreaseBulletNum1")));
		RifleSkill20->SetRequiredEssence(10);
		RifleSkill21->SetSkillText(FText::FromString(TEXT("IncreaseBulletNum2")));
		RifleSkill21->SetRequiredEssence(20);
		RifleSkill22->SetSkillText(FText::FromString(TEXT("IncreaseBulletNum3")));
		RifleSkill22->SetRequiredEssence(30);
	
		RifleSkill30->SetSkillText(FText::FromString(TEXT("SetRateReloadAnimMontage1")));
		RifleSkill30->SetRequiredEssence(10);
		RifleSkill31->SetSkillText(FText::FromString(TEXT("SetRateReloadAnimMontage2")));
		RifleSkill31->SetRequiredEssence(20);
		RifleSkill32->SetSkillText(FText::FromString(TEXT("SetRateReloadAnimMontage3")));
		RifleSkill32->SetRequiredEssence(30);
	
		// Shotgun Skills
		ShotgunSkill00->SetSkillText(FText::FromString(TEXT("SetShotgunActivated")));
		ShotgunSkill00->SetRequiredEssence(10);
	
		ShotgunSkill10->SetSkillText(FText::FromString(TEXT("SetShootInterval1")));
		ShotgunSkill10->SetRequiredEssence(10);
		ShotgunSkill11->SetSkillText(FText::FromString(TEXT("SetShootInterval2")));
		ShotgunSkill11->SetRequiredEssence(20);
		ShotgunSkill12->SetSkillText(FText::FromString(TEXT("SetShootInterval3")));
		ShotgunSkill12->SetRequiredEssence(30);
	
		ShotgunSkill20->SetSkillText(FText::FromString(TEXT("IncreaseBulletNum1")));
		ShotgunSkill20->SetRequiredEssence(10);
		ShotgunSkill21->SetSkillText(FText::FromString(TEXT("IncreaseBulletNum2")));
		ShotgunSkill21->SetRequiredEssence(20);
		ShotgunSkill22->SetSkillText(FText::FromString(TEXT("IncreaseBulletNum3")));
		ShotgunSkill22->SetRequiredEssence(30);
	
		ShotgunSkill30->SetSkillText(FText::FromString(TEXT("SetRateReloadAnimMontage1")));
		ShotgunSkill30->SetRequiredEssence(10);
		ShotgunSkill31->SetSkillText(FText::FromString(TEXT("SetRateReloadAnimMontage2")));
		ShotgunSkill31->SetRequiredEssence(20);
		ShotgunSkill32->SetSkillText(FText::FromString(TEXT("SetRateReloadAnimMontage3")));
		ShotgunSkill32->SetRequiredEssence(30);
	
		// Ultimate Skills
		UltimateSkill00->SetSkillText(FText::FromString(TEXT("SetUltimateActivated")));
		UltimateSkill00->SetRequiredEssence(10);
	
		UltimateSkill10->SetSkillText(FText::FromString(TEXT("UltimateSkill10")));
		UltimateSkill10->SetRequiredEssence(10);
		UltimateSkill11->SetSkillText(FText::FromString(TEXT("UltimateSkill11")));
		UltimateSkill11->SetRequiredEssence(20);
		UltimateSkill12->SetSkillText(FText::FromString(TEXT("UltimateSkill12")));
		UltimateSkill12->SetRequiredEssence(30);
	
		UltimateSkill20->SetSkillText(FText::FromString(TEXT("UltimateSkill20")));
		UltimateSkill20->SetRequiredEssence(10);
		UltimateSkill21->SetSkillText(FText::FromString(TEXT("UltimateSkill21")));
		UltimateSkill21->SetRequiredEssence(20);
		UltimateSkill22->SetSkillText(FText::FromString(TEXT("UltimateSkill22")));
		UltimateSkill22->SetRequiredEssence(30);
	
		UltimateSkill30->SetSkillText(FText::FromString(TEXT("UltimateSkill30")));
		UltimateSkill30->SetRequiredEssence(10);
		UltimateSkill31->SetSkillText(FText::FromString(TEXT("UltimateSkill31")));
		UltimateSkill31->SetRequiredEssence(20);
		UltimateSkill32->SetSkillText(FText::FromString(TEXT("UltimateSkill32")));
		UltimateSkill32->SetRequiredEssence(30);
	
		// Hammer Skills
		HammerSkill00->SetSkillText(FText::FromString(TEXT("SetHammerActivated")));
		HammerSkill00->SetRequiredEssence(10);
	
		HammerSkill10->SetSkillText(FText::FromString(TEXT("HammerSkill10")));
		HammerSkill10->SetRequiredEssence(10);
		HammerSkill11->SetSkillText(FText::FromString(TEXT("HammerSkill11")));
		HammerSkill11->SetRequiredEssence(20);
		HammerSkill12->SetSkillText(FText::FromString(TEXT("HammerSkill12")));
		HammerSkill12->SetRequiredEssence(30);
	
		HammerSkill20->SetSkillText(FText::FromString(TEXT("HammerSkill20")));
		HammerSkill20->SetRequiredEssence(10);
		HammerSkill21->SetSkillText(FText::FromString(TEXT("HammerSkill21")));
		HammerSkill21->SetRequiredEssence(20);
		HammerSkill22->SetSkillText(FText::FromString(TEXT("HammerSkill22")));
		HammerSkill22->SetRequiredEssence(30);
	
		HammerSkill30->SetSkillText(FText::FromString(TEXT("HammerSkill30")));
		HammerSkill30->SetRequiredEssence(10);
		HammerSkill31->SetSkillText(FText::FromString(TEXT("HammerSkill31")));
		HammerSkill31->SetRequiredEssence(20);
		HammerSkill32->SetSkillText(FText::FromString(TEXT("HammerSkill32")));
		HammerSkill32->SetRequiredEssence(30);
	*/

	// APuckPlayerState 가져오기
	APuckPlayerState* PlayerState = GetWorld()->GetFirstPlayerController()->GetPlayerState<APuckPlayerState>();
	if (!PlayerState)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerState is null"));
		return;
	}

	// 모든 SkillTemplate의 OnSkillButtonClicked에 바인딩
	TArray<USkillTemplate*> SkillTemplates = {
		RifleSkill00, RifleSkill10, RifleSkill11, RifleSkill12, RifleSkill20, RifleSkill21, RifleSkill22, RifleSkill30,
		RifleSkill31, RifleSkill32,
		ShotgunSkill00, ShotgunSkill10, ShotgunSkill11, ShotgunSkill12, ShotgunSkill20, ShotgunSkill21, ShotgunSkill22,
		ShotgunSkill30, ShotgunSkill31, ShotgunSkill32,
		UltimateSkill00, UltimateSkill10, UltimateSkill11, UltimateSkill12, UltimateSkill20, UltimateSkill21,
		UltimateSkill22, UltimateSkill30, UltimateSkill31, UltimateSkill32,
		HammerSkill00, HammerSkill10, HammerSkill11, HammerSkill12, HammerSkill20, HammerSkill21, HammerSkill22,
		HammerSkill30, HammerSkill31, HammerSkill32
	};

	auto RowNames = SkillDataTable->GetRowNames();

	for (int32 i = 0; i < SkillTemplates.Num() && i < RowNames.Num(); ++i)
	{
		// SkillDataTable 에서 SkillData 를 가져와 SkillTemplate 에 SetSkillData
		FSkillData* SkillData = SkillDataTable->FindRow<FSkillData>(RowNames[i], TEXT(""));
		SkillTemplates[i]->SetSkillData(*SkillData);
	}

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

	// 00 스킬은 Assignable
	RifleSkill00->SetAssignable(true);
	ShotgunSkill00->SetAssignable(true);
	UltimateSkill00->SetAssignable(true);
	HammerSkill00->SetAssignable(true);
}

void USkillWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// 마우스 스크롤 위로 올리면, CanvasPanel_Skills의 위치를 올림
	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::MouseScrollUp))
	{
		SetCanvasPanel_SkillsPositionY(true);
	}
	// 마우스 스크롤 아래로 내리면, CanvasPanel_Skills의 위치를 내림
	else if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::MouseScrollDown))
	{
		SetCanvasPanel_SkillsPositionY(false);
	}
}

void USkillWidget::SetEssenceCount(const int32 Essence)
{
	EssenceCount->SetText(FText::FromString(FString::FromInt(Essence)));
}

void USkillWidget::SetCanvasPanel_SkillsPositionY(bool bIsUp)
{
	if (bIsUp)
	{
		SetCurrentYTransform(CurrentYTransform + 50.0f);
	}
	else
	{
		SetCurrentYTransform(CurrentYTransform - 50.0f);
	}

	SetAllChildYTransform(CurrentYTransform);
}

void USkillWidget::SetAllChildYTransform(float NewYTransform)
{
	NewYTransform = FMath::Clamp(NewYTransform, -YRange, YRange);

	// 모든 자식들의 위치를 변경
	for (auto* Child : CanvasPanel_Skills->GetAllChildren())
	{
		if (Child != Slider_Scroll)
			Child->SetRenderTranslation(FVector2D(0.0f, NewYTransform));
	}
}

void USkillWidget::SetCurrentYTransform_Implementation(float NewYTransform)
{
	// CurrentYTransform 값의 범위를 -200.0f ~ 200.0f 로 제한
	// CurrentYTransform = FMath::Clamp(NewYTransform, -200.0f, 200.0f);
}
