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
	class UStatusComponent* Status;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* MaxHPValue;

	UFUNCTION()
	void SetStatusValueInWidget();
};
