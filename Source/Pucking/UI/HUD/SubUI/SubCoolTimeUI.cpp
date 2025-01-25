// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SubUI/SubCoolTimeUI.h"

#include "Components/ProgressBar.h"

void USubCoolTimeUI::NativeConstruct()
{
	Super::NativeConstruct();

	JetpackCoolTimeBar->SetPercent(1.f);
	HookCoolTimeBar->SetPercent(1.f);
}

void USubCoolTimeUI::SetJetpackGauge(float Percent)
{
	JetpackCoolTimeBar->SetPercent(Percent);
	if(Percent <= 0.06f)
	{
		SetJetpackCoolDown();
	}
}

void USubCoolTimeUI::SetHookGauge(float Percent)
{
	HookCoolTimeBar->SetPercent(Percent);
}

void USubCoolTimeUI::SetJetpackCoolDown()
{
	if(JetpackCoolTimeAnim && !IsPlayingAnimation())
	{
		PlayAnimation(JetpackCoolTimeAnim, 0.f, 0.f);
	}
}

void USubCoolTimeUI::SetHookCoolDown()
{
}

void USubCoolTimeUI::SetJetpackReady()
{
	if(IsPlayingAnimation())
	{
		StopAnimation(JetpackCoolTimeAnim);
	}
}

void USubCoolTimeUI::SetHookReady()
{
}
