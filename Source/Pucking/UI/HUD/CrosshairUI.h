// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/SpringArmComponent.h"
#include "CrosshairUI.generated.h"

class UBorder;
class UCanvasPanelSlot;
/**
 * 
 */
UCLASS()
class PUCKING_API UCrosshairUI : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeOnInitialized() override;
	
	virtual void NativeConstruct() override;

private:
	// Player Camera SpringArm
	UPROPERTY()
	USpringArmComponent* PlayerSpringArmComponent;
	
	// Border UI
	UPROPERTY(meta = (BindWidget))
	UBorder* NorthBorder;

	UPROPERTY(meta = (BindWidget))
	UBorder* SouthBorder;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* WestBorder;
	
	UPROPERTY(meta = (BindWidget))
	UBorder* EastBorder;

	// Border UI Slot
	UPROPERTY()
	UCanvasPanelSlot* NorthCanvasSlot;

	UPROPERTY()
	UCanvasPanelSlot* SouthCanvasSlot;

	UPROPERTY()
	UCanvasPanelSlot* WestCanvasSlot;

	UPROPERTY()
	UCanvasPanelSlot* EastCanvasSlot;
	
	// 스프링암 초기 길이
	UPROPERTY(VisibleAnywhere)
	float SpringArmLength;

	// 줌 했을 때 모이는 정도
	UPROPERTY(EditAnywhere)
	float ShortenZoomCrosshair = 20.f;

public:
	// Crosshair UI 집탄율 변화
	UFUNCTION()
	void SetCrosshairPosition(float Value);
	
	// 조준 상태일 때
	void ChangeAimingCrosshair();
	
	// 기본 상태일 때
	void ChangeDefaultCrosshair();
	
	// 스프링암 거리 줄임
	void ZoomInCrosshair();

	// 스프링암 거리 원래대로
	void ZoomOutCrosshair();
};
