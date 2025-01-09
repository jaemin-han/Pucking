// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShotgunUI.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API UShotgunUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UImage* ShotgunCrosshair;
	
	// Player Camera SpringArm
	UPROPERTY()
	class USpringArmComponent* PlayerSpringArmComponent;

	// 스프링암 초기 길이
	UPROPERTY(VisibleAnywhere)
	float SpringArmLength;

	// 줌 했을 때 모이는 정도
	UPROPERTY(EditAnywhere)
	float ShortenZoomCrosshair = 20.f;

public:
	// 스프링암 거리 줄임
	void ZoomInCrosshair();

	// 스프링암 거리 원래대로
	void ZoomOutCrosshair();
};
