// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/CrosshairUI.h"

#include "Components/Border.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CanvasPanelSlot.h"

void UCrosshairUI::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
}

void UCrosshairUI::NativeConstruct()
{
	Super::NativeConstruct();

	// 플레이어가 한 명만 있을 때의 구조
	if(GetWorld())
	{
		APawn* Player = GetWorld()->GetFirstPlayerController()->GetPawn();
		if(Player)
		{
			PlayerSpringArmComponent = Player->GetComponentByClass<USpringArmComponent>();
			SpringArmLength = PlayerSpringArmComponent->TargetArmLength;
		}
	}

	// CanvasSlot 캐싱
	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(NorthBorder->Slot))
	{
		NorthCanvasSlot = CanvasSlot;
	}

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(SouthBorder->Slot))
	{
		SouthCanvasSlot = CanvasSlot;
	}

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(WestBorder->Slot))
	{
		WestCanvasSlot = CanvasSlot;
	}

	if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(EastBorder->Slot))
	{
		EastCanvasSlot = CanvasSlot;
	}
	
}

// 탄 퍼짐에 따라 UI 변경 
void UCrosshairUI::SetCrosshairPosition(float Value)
{
	if(NorthBorder)
	{
		NorthBorder->SetRenderTranslation(FVector2D(NorthBorder->GetRenderTransform().Translation.X, Value * -1));
	}

	if(SouthBorder)
	{
		SouthBorder->SetRenderTranslation(FVector2D(SouthBorder->GetRenderTransform().Translation.X, Value));
	}

	if(WestBorder)
	{
		WestBorder->SetRenderTranslation(FVector2D(Value * -1, WestBorder->GetRenderTransform().Translation.Y));
	}

	if(EastBorder)
	{
		EastBorder->SetRenderTranslation(FVector2D(Value, EastBorder->GetRenderTransform().Translation.Y));
	}
	
}

// 조준 상태일 때
void UCrosshairUI::ChangeAimingCrosshair()
{
	if (NorthCanvasSlot)
	{
		FVector2D OriginFVector = NorthCanvasSlot->GetPosition();
		NorthCanvasSlot->SetPosition(FVector2D(OriginFVector.X, OriginFVector.Y + ShortenZoomCrosshair));
	}

	if (SouthCanvasSlot)
	{
		FVector2D OriginFVector = SouthCanvasSlot->GetPosition();
		SouthCanvasSlot->SetPosition(FVector2D(OriginFVector.X, OriginFVector.Y - ShortenZoomCrosshair));
	}

	if (WestCanvasSlot)
	{
		FVector2D OriginFVector = WestCanvasSlot->GetPosition();
		WestCanvasSlot->SetPosition(FVector2D(OriginFVector.X + ShortenZoomCrosshair, OriginFVector.Y));
	}

	if (EastCanvasSlot)
	{
		FVector2D OriginFVector = EastCanvasSlot->GetPosition();
		EastCanvasSlot->SetPosition(FVector2D(OriginFVector.X - ShortenZoomCrosshair, OriginFVector.Y));
	}
}

// 기본 상태일 때
void UCrosshairUI::ChangeDefaultCrosshair()
{
	if (NorthCanvasSlot)
	{
		FVector2D OriginFVector = NorthCanvasSlot->GetPosition();
		NorthCanvasSlot->SetPosition(FVector2D(OriginFVector.X, OriginFVector.Y - ShortenZoomCrosshair));
	}

	if (SouthCanvasSlot)
	{
		FVector2D OriginFVector = SouthCanvasSlot->GetPosition();
		SouthCanvasSlot->SetPosition(FVector2D(OriginFVector.X, OriginFVector.Y + ShortenZoomCrosshair));
	}

	if (WestCanvasSlot)
	{
		FVector2D OriginFVector = WestCanvasSlot->GetPosition();
		WestCanvasSlot->SetPosition(FVector2D(OriginFVector.X - ShortenZoomCrosshair, OriginFVector.Y));
	}

	if (EastCanvasSlot)
	{
		FVector2D OriginFVector = EastCanvasSlot->GetPosition();
		EastCanvasSlot->SetPosition(FVector2D(OriginFVector.X + ShortenZoomCrosshair, OriginFVector.Y));
	}
}

// Zoom 시작
void UCrosshairUI::ZoomInCrosshair()
{
	// 스프링암 길이 축소
	if(PlayerSpringArmComponent)
	{
		PlayerSpringArmComponent->TargetArmLength = (SpringArmLength * 0.2);
		ChangeAimingCrosshair();
	}
	
}

// Zoom 끝
void UCrosshairUI::ZoomOutCrosshair()
{
	// 스프링암 길이 원래대로
	if(PlayerSpringArmComponent)
	{
		PlayerSpringArmComponent->TargetArmLength = SpringArmLength;
		ChangeDefaultCrosshair();
	}
	
}
