// Fill out your copyright notice in the Description page of Project Settings.


#include "World/PuckMainMenuGameMode.h"
#include "Kismet/GameplayStatics.h"

APuckMainMenuGameMode::APuckMainMenuGameMode()
{
}

void APuckMainMenuGameMode::BeginPlay()
{
	if (MainMenuWidgetClass)
	{
		UUserWidget* MainMenu = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidgetClass);
		if (MainMenu)
		{
			MainMenu->AddToViewport();
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PC)
			{
				PC->bShowMouseCursor = true;
			}
		}
	}
}
