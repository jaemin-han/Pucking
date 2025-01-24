// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "GameOverUI.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API UGameOverUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPuckGameInstance* PuckGameInstance;
	UPROPERTY(meta = (BindWidget))
	UButton* Restart_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* MainMenu_Btn;

	UFUNCTION(BlueprintCallable)
	void OnRestartButtonClicked();
	UFUNCTION(BlueprintCallable)
	void OnMainMenuButtonClicked();
};
