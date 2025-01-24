// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuUI.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API UMainMenuUI : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
private:
	UFUNCTION()
	void OnStartButtonClicked();
public:
	UPROPERTY(meta = (BindWidget))
	UButton* GameStart_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Exit_Btn;


};
