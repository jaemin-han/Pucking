// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Enemy/HealthBarComponent.h"

#include "HealthBarUI.h"
#include "Components/ProgressBar.h"

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