// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Common/CommonEnum.h"
#include "Engine/DataAsset.h"
#include "OptionDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API UOptionDataAsset : public UPrimaryDataAsset
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

	// option weight (int32)
	UPROPERTY(EditAnywhere, Category = "Option")
	int32 OptionWeight;

	// option description
	UPROPERTY(EditAnywhere, Category = "Option")
	FString OptionDescription;

public:
	void SetOptionDescription();
	UFUNCTION(BlueprintCallable, Category = "Option")
	FString GetOptionDescription();

	/* 설명
	 *	OptionDataAssetArray 에서 ItemTier 이하인 옵션 중 ItemRarity에 맞게 옵션의 개수를 랜덤하게 선택하여 반환
	 *	
	 *	input:
	 *		OptionDataAssetArray: TArray<UOptionDataAsset*> 형태의 OptionDataAsset Array
	 *			모든 아이템 옵션이 들어 있음
	 *		Item Tier: int32 형태의 Item Tier
	 *			아이템의 티어, 해당 티어 이하인 옵션만 선택될 수 있음
	 *		Item Rarity: EItemRarity 형태의 Item Rarity
	 *			일반 아이템의 경우 옵션이 0개, 매직 아이템의 경우 옵션이 1 ~ 2개, 레어 아이템의 경우 옵션이 3 ~ 4개
	 *
	 *	return:
	 *		 TArray<UOptionDataAsset*> 형태의 SelectedOptions
	 */
	UFUNCTION(BlueprintCallable, Category = "Option")
	static TArray<UOptionDataAsset*> GetRandomOptions(const TArray<UOptionDataAsset*>& OptionDataAssetArray,
	                                                  const int32 ItemTier, const EItemRarity ItemRarity);
};
