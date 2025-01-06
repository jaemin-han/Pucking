// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReticleUI.generated.h"

class UBorder;
class UCanvasPanelSlot;
/**
 * 
 */
UCLASS()
class PUCKING_API UReticleUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	UBorder* NorthBorder;

	UPROPERTY()
	UCanvasPanelSlot* NorthCanvasPanelSlot;

	UPROPERTY(meta = (BindWidget))
	UBorder* SouthBorder;

	UPROPERTY()
	UCanvasPanelSlot* SouthCanvasPanelSlot;

	UPROPERTY(meta = (BindWidget))
	UBorder* WestBorder;

	UPROPERTY()
	UCanvasPanelSlot* WestCanvasPanelSlot;

	UPROPERTY(meta = (BindWidget))
	UBorder* EastBorder;
	
	UPROPERTY()
	UCanvasPanelSlot* EastCanvasPanelSlot;

public:
	UFUNCTION()
	void SetReticlePosition(float Value);
	
};
