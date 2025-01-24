// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "UI/Status/MainMenuUI.h"
#include "PuckMainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API APuckMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
public:
	APuckMainMenuGameMode();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="MainMenuUI")
	TSubclassOf<UMainMenuUI> MainMenuWidgetClass;
};
