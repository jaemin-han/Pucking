// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Enemy/HealthBarComponent.h"

#include "HealthBarUI.h"
#include "Components/ProgressBar.h"
#include "Components/Image.h"

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	if(HealthBarUI == nullptr)
	{
		HealthBarUI = Cast<UHealthBarUI>(GetUserWidgetObject());
	}
	if(HealthBarUI)
	{
		HealthBarUI->HealthBar->SetPercent(Percent);
	}
}

void UHealthBarComponent::SetShieldPercent(float Percent)
{
	if(HealthBarUI == nullptr)
	{
		HealthBarUI = Cast<UHealthBarUI>(GetUserWidgetObject());
	}
	if(HealthBarUI)
	{
		HealthBarUI->ShieldBar->SetPercent(Percent);
	}
}

void UHealthBarComponent::SetTypeImage(EDamageType Type)
{
	if(HealthBarUI == nullptr)
	{
		HealthBarUI = Cast<UHealthBarUI>(GetUserWidgetObject());
	}
	if(HealthBarUI)
	{
		switch(Type)
		{
		case EDamageType::Fire:
			HealthBarUI->Fire_Image->SetVisibility(ESlateVisibility::Visible);
			HealthBarUI->Ice_Image->SetVisibility(ESlateVisibility::Hidden);
			HealthBarUI->Normal_Image->SetVisibility(ESlateVisibility::Hidden);
		case EDamageType::Ice:
			HealthBarUI->Fire_Image->SetVisibility(ESlateVisibility::Hidden);
			HealthBarUI->Ice_Image->SetVisibility(ESlateVisibility::Visible);
			HealthBarUI->Normal_Image->SetVisibility(ESlateVisibility::Hidden);
		case EDamageType::Physical:
			HealthBarUI->Fire_Image->SetVisibility(ESlateVisibility::Hidden);
			HealthBarUI->Ice_Image->SetVisibility(ESlateVisibility::Hidden);
			HealthBarUI->Normal_Image->SetVisibility(ESlateVisibility::Visible);
		}
	}
}