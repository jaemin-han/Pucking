// Fill out your copyright notice in the Description page of Project Settings.


#include "PuckPlayerState.h"

void APuckPlayerState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	GEngine->AddOnScreenDebugMessage(512312, 0.0f, FColor::Red, FString::Printf(TEXT("Essence: %d"), Essence));
}

APuckPlayerState::APuckPlayerState()
{
	// tick true
	PrimaryActorTick.bCanEverTick = true;
	Essence = 0;
}
