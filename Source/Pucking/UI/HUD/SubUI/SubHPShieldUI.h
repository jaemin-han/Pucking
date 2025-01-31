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
	virtual void NativeOnInitialized() override;

public:
	//PlayerStatus Cast
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UPlayerStatusComponent* PlayerStatus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class APuckingCharacter* PuckCharacter;


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
	class UProgressBar* HealthProgressBar;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* ShieldProgressBar;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int32 MaxHealth = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(AllowPrivateAccess=true))
	int32 MaxShield = 500;

	UPROPERTY(EditAnywhere)
	float HPPercent;

	UPROPERTY(EditAnywhere)
	float ShieldPercent;

public:
	// 체력 UI 변경
	UFUNCTION(BlueprintCallable)
	void SetHealthUI(int32 HealthMount);

	// 실드 UI 변경
	UFUNCTION(BlueprintCallable)
	void SetShieldUI(int32 ShieldMount);

	//HP ProgressBar
	UFUNCTION(BlueprintCallable)
	void UpdateHPProgressBar(float RemainHp);
	//Shield ProgressBar
	UFUNCTION(BlueprintCallable)
	void UpdateShieldProgressBar(float RemainShield);

	UFUNCTION(BlueprintCallable)
	void UpdateProgress();
};
