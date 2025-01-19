// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SubHUD.generated.h"

enum class EWeaponType : uint8;
class UTextBlock;
/**
 * 
 */
UCLASS()
class PUCKING_API USubHUD : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	/*UPROPERTY(meta = (BindWidget))
	class UImage* RifleImage;

	UPROPERTY(meta = (BindWidget))
	class UImage* ShotgunImage;*/
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CurMagazine;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* MaxMagazine;

public:
	// 무기 교체 시
	void ChangeWeaponImg(EWeaponType WeaponType);
	
	// 총 소비
	void ChangeCurMagazine(int32 CurrentM);

	// 탄창 변경
	void ChangeMaxMagazine(int32 MaxM);
};
