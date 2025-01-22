// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API UPlayerStatusWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UPlayerStatusComponent* Status;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class APuckingCharacter* PuckCharacter;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxHPTextBlock;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DefenseTextBlock;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* DamageTextBlock;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* CriticalChanceTextBlock;

	UFUNCTION()
	void SetStatusValueInWidget();

	UFUNCTION(BlueprintCallable)
	EDamageType GetCharacterDamageType();

	UFUNCTION(BlueprintCallable)
	FString GetCharacterDamageTypeText();
};
