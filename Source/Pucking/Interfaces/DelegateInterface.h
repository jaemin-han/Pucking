// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DelegateInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UDelegateInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PUCKING_API IDelegateInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual FDelegateHandle DelegateFireComplete(const TDelegate<void(int32)>& Delegate) = 0;
	virtual FDelegateHandle DelegateReloadComplete(const TDelegate<void(int32)>& Delegate) = 0;
	//virtual void BroadcastDelegateEvent(int32 Value) = 0;
};
