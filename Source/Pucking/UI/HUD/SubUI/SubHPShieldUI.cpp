// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SubUI/SubHPShieldUI.h"

#include "Components/TextBlock.h"

void USubHPShieldUI::NativeConstruct()
{
	Super::NativeConstruct();

	SetHealthUI(MaxHealth);
	SetShieldUI(MaxShield);
}

void USubHPShieldUI::SetHealthUI(int32 HealthMount)
{
	HealthText->SetText(FText::AsNumber(HealthMount));
}

void USubHPShieldUI::SetShieldUI(int32 ShieldMount)
{
	ShieldText->SetText(FText::AsNumber(ShieldMount));
}
