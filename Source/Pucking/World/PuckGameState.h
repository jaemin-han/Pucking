// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "PuckGameState.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API APuckGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	// Material Array
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "PuckGameState")
	TArray<class UMaterialInterface*> MaterialArray;
};
