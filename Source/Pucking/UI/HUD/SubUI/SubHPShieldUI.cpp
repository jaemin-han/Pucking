// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/SubUI/SubHPShieldUI.h"

#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

#include "Character/PuckingCharacter.h"
#include "ActorComponent/PlayerStatusComponent.h"
#include "Kismet/GameplayStatics.h"

void USubHPShieldUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	PuckCharacter = Cast<APuckingCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	PlayerStatus = PuckCharacter->FindComponentByClass<UPlayerStatusComponent>();


	PlayerStatus->OnCharacterHPShieldChanged.AddDynamic(this, &USubHPShieldUI::UpdateProgress);
}

void USubHPShieldUI::SetHealthUI(int32 HealthMount)
{
	HealthText->SetText(FText::AsNumber(HealthMount));
}

void USubHPShieldUI::SetShieldUI(int32 ShieldMount)
{
	ShieldText->SetText(FText::AsNumber(ShieldMount));
}

void USubHPShieldUI::UpdateHPProgressBar(float RemainHp)
{
	if (HealthProgressBar)
	{
		HealthProgressBar->SetPercent(FMath::Clamp(RemainHp, 0.0f, 1.0f));
	}
}

void USubHPShieldUI::UpdateShieldProgressBar(float RemainShield)
{
	if (ShieldProgressBar)
	{
		ShieldProgressBar->SetPercent(FMath::Clamp(RemainShield, 0.0f, 1.0f));
	}
}

void USubHPShieldUI::UpdateProgress()
{
	HPPercent = PlayerStatus->RemainHP / PlayerStatus->CurMaxHP;
	ShieldPercent = PlayerStatus->RemainShield / PlayerStatus->CurMaxShield;
	UpdateHPProgressBar(HPPercent);
	UpdateShieldProgressBar(ShieldPercent);
}
