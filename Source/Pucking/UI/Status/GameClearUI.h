// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameClearUI.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API UGameClearUI : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPuckGameInstance* PuckGameInstance;
	UPROPERTY(meta = (BindWidget))
	class UButton* Restart_Btn;
	UPROPERTY(meta = (BindWidget))
	class UButton* Exit_Btn;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TotalScoreText;


	UFUNCTION(BlueprintCallable)
	void OnRestartButtonClicked();
	UFUNCTION(BlueprintCallable)
	void OnMainExitButtonClicked();
	UFUNCTION(BlueprintCallable)
	void SetTotalScoreInTextBlock();
};
