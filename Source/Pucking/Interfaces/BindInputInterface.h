// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BindInputInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UBindInputInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PUCKING_API IBindInputInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	virtual TArray<struct FInputParameter> ReturnInputParameter() = 0;
};
