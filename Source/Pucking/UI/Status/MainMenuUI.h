// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuUI.generated.h"

class UOverlay;
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
	UFUNCTION()
	void OnEndButtonClicked();
	UFUNCTION()
	void ConfirmButtonClicked();
	UFUNCTION()
	void CancelButtonClicked();
public:
	UPROPERTY(meta = (BindWidget))
	UButton* GameStart_Btn;
	UPROPERTY(meta = (BindWidget))
	UButton* Exit_Btn;

	// 게임종료 확인 팝업
	UPROPERTY(meta=(BindWidget))
	UOverlay* ConfirmPopup;

	// 팝업 확인 버튼
	UPROPERTY(meta=(BindWidget))
	UButton* ConfirmButton;

	// 팝업 취소 버튼
	UPROPERTY(meta=(BindWidget))
	UButton* CancelButton;
};
