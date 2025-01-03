// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ReticleUI.generated.h"

class UBorder;
/**
 * 
 */
UCLASS()
class PUCKING_API UReticleUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	UBorder* NorthBorder;

	UPROPERTY(meta = (BindWidget))
	UBorder* SouthBorder;

	UPROPERTY(meta = (BindWidget))
	UBorder* WestBorder;

	UPROPERTY(meta = (BindWidget))
	UBorder* EastBorder;
};
