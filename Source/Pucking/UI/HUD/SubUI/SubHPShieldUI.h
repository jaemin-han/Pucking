// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SubHPShieldUI.generated.h"

class UProgressBar;
class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PUCKING_API USubHPShieldUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	// HP
	UPROPERTY(meta = (BindWidget))
	UTextBlock* HealthText;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* ShieldText;

	// Icon Image
	UPROPERTY(meta = (BindWidget))
	UImage* HealthIconImg;

	UPROPERTY(meta = (BindWidget))
	UImage* ShieldIconImg;

	// Progressbar
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* ShieldProgressBar;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int32 MaxHealth = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int32 MaxShield = 500;

public:
	// 체력 UI 변경
	UFUNCTION(BlueprintCallable)
	void SetHealthUI(int32 HealthMount);

	// 실드 UI 변경
	UFUNCTION(BlueprintCallable)
	void SetShieldUI(int32 ShieldMount);
};
