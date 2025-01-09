// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/ShotgunUI.h"

#include "GameFramework/SpringArmComponent.h"

void UShotgunUI::NativeConstruct()
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
}

void UShotgunUI::ZoomInCrosshair()
{
	// 스프링암 길이 축소
	if(PlayerSpringArmComponent)
	{
		PlayerSpringArmComponent->TargetArmLength = (SpringArmLength * 0.2);
	}
}

void UShotgunUI::ZoomOutCrosshair()
{
	// 스프링암 길이 원래대로
	if(PlayerSpringArmComponent)
	{
		PlayerSpringArmComponent->TargetArmLength = SpringArmLength;
	}
}
