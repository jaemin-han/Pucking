// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Status/MainMenuUI.h"

#include "Components/Overlay.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (GameStart_Btn)
	{
		GameStart_Btn->OnClicked.AddDynamic(this, &UMainMenuUI::OnStartButtonClicked);
	}

	if (Exit_Btn)
	{
		Exit_Btn->OnClicked.AddDynamic(this, &UMainMenuUI::OnEndButtonClicked);
	}
	
	if (ConfirmButton)
	{
		ConfirmButton->OnClicked.AddDynamic(this, &UMainMenuUI::ConfirmButtonClicked);
	}
	
	if (CancelButton)
	{
		CancelButton->OnClicked.AddDynamic(this, &UMainMenuUI::CancelButtonClicked);
	}


	if(GetWorld() && GetWorld()->GetFirstPlayerController())
	{
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
	}

	ConfirmPopup->SetVisibility(ESlateVisibility::Hidden);
}

void UMainMenuUI::OnStartButtonClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Lv_GameMap"));
}

void UMainMenuUI::OnEndButtonClicked()
{
	ConfirmPopup->SetVisibility(ESlateVisibility::Visible);

	GameStart_Btn->SetIsEnabled(false);
	Exit_Btn->SetIsEnabled(false);
}

void UMainMenuUI::ConfirmButtonClicked()
{
	if(GetWorld() && GetWorld()->GetFirstPlayerController())
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
	}
}

void UMainMenuUI::CancelButtonClicked()
{
	ConfirmPopup->SetVisibility(ESlateVisibility::Hidden);

	GameStart_Btn->SetIsEnabled(true);
	Exit_Btn->SetIsEnabled(true);
}
