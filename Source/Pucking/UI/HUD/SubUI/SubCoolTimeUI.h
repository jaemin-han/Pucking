// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "SubCoolTimeUI.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API USubCoolTimeUI : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* JetpackCoolTimeBar;
	
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* JetpackCoolTimeAnim;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* HookCoolTimeBar;

public:
	void SetJetpackGauge(float Percent);
	void SetHookGauge(float Percent);

	void SetJetpackReady();
	void SetHookReady();

private:
	void SetJetpackCoolDown();
	void SetHookCoolDown();
};
