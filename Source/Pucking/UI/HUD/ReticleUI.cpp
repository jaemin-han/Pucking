// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ReticleUI.h"

#include "Components/Border.h"
#include "Components/CanvasPanelSlot.h"

void UReticleUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
}

void UReticleUI::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(NorthBorder->Slot))
	{
		NorthCanvasPanelSlot = CanvasSlot;
		NorthCanvasPanelSlot->SetSize(FVector2D(5,60));
		NorthCanvasPanelSlot->SetPosition(FVector2D(960,400));
	}
	
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(SouthBorder->Slot))
	{
		SouthCanvasPanelSlot = CanvasSlot;
		SouthCanvasPanelSlot->SetSize(FVector2D(5,60));
		SouthCanvasPanelSlot->SetPosition(FVector2D(960,570));
	}
	
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(WestBorder->Slot))
	{
		WestCanvasPanelSlot = CanvasSlot;
		WestCanvasPanelSlot->SetSize(FVector2D(60,5));
		WestCanvasPanelSlot->SetPosition(FVector2D(850,515));
	}

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(EastBorder->Slot))
	{
		EastCanvasPanelSlot = CanvasSlot;
		EastCanvasPanelSlot->SetSize(FVector2D(60,5));
		EastCanvasPanelSlot->SetPosition(FVector2D(1010,515));
	}
}

void UReticleUI::SetReticlePosition(float Value)
{
	if(NorthCanvasPanelSlot)
	{
		FVector2d OriginNorthPos = NorthCanvasPanelSlot->GetPosition();
		NorthCanvasPanelSlot->SetPosition(FVector2d(OriginNorthPos.X, OriginNorthPos.Y - Value));
	}
	
	if(SouthCanvasPanelSlot)
	{
		FVector2d OriginSouthPos = SouthCanvasPanelSlot->GetPosition();
		SouthCanvasPanelSlot->SetPosition(FVector2d(OriginSouthPos.X, OriginSouthPos.Y + Value));
	}

	if(WestCanvasPanelSlot)
	{
		FVector2d OriginWestPos = WestCanvasPanelSlot->GetPosition();
		WestCanvasPanelSlot->SetPosition(FVector2d(OriginWestPos.X - Value, OriginWestPos.Y));
	}
	
	if(EastCanvasPanelSlot)
	{
		FVector2d OriginEastPos = EastCanvasPanelSlot->GetPosition();
		EastCanvasPanelSlot->SetPosition(FVector2d(OriginEastPos.X + Value, OriginEastPos.Y));
	}
}
