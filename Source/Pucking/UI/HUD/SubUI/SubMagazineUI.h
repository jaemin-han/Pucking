// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SubMagazineUI.generated.h"

enum class EWeaponType : uint8;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PUCKING_API USubMagazineUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurMagazine;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxMagazine;

public:
	// 총 사용
	void ChangeCurMagazine(int32 CurrentM);

	// 탄창 변경
	void ChangeMaxMagazine(int32 MaxM);
};
