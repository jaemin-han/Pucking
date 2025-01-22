// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Status/PlayerStatusWidget.h"
#include "ActorComponent/PlayerStatusComponent.h"
#include "Components/TextBlock.h"
#include "Character/PuckingCharacter.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void UPlayerStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PuckCharacter = Cast<APuckingCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	Status = PuckCharacter->FindComponentByClass<UPlayerStatusComponent>();

}

void UPlayerStatusWidget::SetStatusValueInWidget()
{
	
}

EDamageType UPlayerStatusWidget::GetCharacterDamageType()
{
	if (PuckCharacter)
	{
		if (Status)
		{
			return Status->CommonDamageType;

		}
		else
		{
			return EDamageType::Physical;
		}
	}
	return EDamageType::Physical;
}

FString UPlayerStatusWidget::GetCharacterDamageTypeText()
{
	switch (GetCharacterDamageType())
	{
	case EDamageType::Physical:
		return "Physical";
		break;
	case EDamageType::Fire:
		return "Fire";
		break;
	case EDamageType::Ice:
		return "Ice";
		break;
	default:
		return "Unknown";
		break;
	}
}

