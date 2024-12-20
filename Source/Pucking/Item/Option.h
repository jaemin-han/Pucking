// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Common/CommonEnum.h"
#include "Option.generated.h"

/**
 * 
 */
// todo: 추후에 struct로 변경해도 될 것 같음
UCLASS()
class PUCKING_API UOption : public UObject
{
	GENERATED_BODY()

	// EOptionType
	UPROPERTY(EditAnywhere, Category = "Option")
	EOptionType OptionType;

	// option value
	UPROPERTY(EditAnywhere, Category = "Option")
	float OptionValue;

	// option tier (int32)
	UPROPERTY(EditAnywhere, Category = "Option")
	int32 OptionTier;

	// option description
	UPROPERTY(EditAnywhere, Category = "Option")
	FString OptionDescription;

public:
	void SetOptionDescription();
	UFUNCTION(BlueprintCallable, Category = "Option")
	FString GetOptionDescription();
};
