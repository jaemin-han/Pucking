// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthMarbleInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UHealthMarbleInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PUCKING_API IHealthMarbleInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ApplyHeal(float HealAmount) = 0;
};
