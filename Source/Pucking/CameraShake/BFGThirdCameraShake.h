// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Shakes/LegacyCameraShake.h"
#include "BFGThirdCameraShake.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API UBFGThirdCameraShake : public ULegacyCameraShake
{
	GENERATED_BODY()

public:
	UBFGThirdCameraShake();

	void StartShake();
};
