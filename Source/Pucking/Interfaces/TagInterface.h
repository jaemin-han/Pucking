// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TagInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UTagInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PUCKING_API ITagInterface
{
	GENERATED_BODY()

public:
	virtual bool HasTag(const FName& Tag) const = 0;
	virtual void AddTag(const FName& Tag) = 0;
	virtual void RemoveTag(const FName& Tag) = 0;

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
