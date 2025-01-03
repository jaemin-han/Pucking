// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionDataAsset.h"

FString UOptionDataAsset::GetDamageTypeDescription()
{
	FString Description;
	switch (static_cast<EDamageType>(OptionValue))
	{
	case EDamageType::Physical:
		Description = TEXT("물리");
		break;
	case EDamageType::Fire:
		Description = TEXT("화염");
		break;
	case EDamageType::Ice:
		Description = TEXT("냉기");
		break;
	default:
		Description = TEXT("알 수 없는 데미지 타입");
		break;
	}
	return Description;
}

void UOptionDataAsset::SetOptionDescription()
{
	switch (OptionType)
	{
	case EOptionType::DamageType:
		OptionDescription = FString::Printf(TEXT("데미지 타입: %s"), *GetDamageTypeDescription());
		break;
	case EOptionType::Damage:
		OptionDescription = FString::Printf(TEXT("데미지: %.1f"), OptionValue);
		break;
	case EOptionType::CriticalRate:
		OptionDescription = FString::Printf(TEXT("치명타 확률: %.1f%%"), OptionValue);
		break;
	case EOptionType::CriticalMultiplier:
		OptionDescription = FString::Printf(TEXT("치명타 배율: %.1f"), OptionValue);
		break;
	case EOptionType::PhysicalPenetration:
		OptionDescription = FString::Printf(TEXT("물리 관통력: %.1f"), OptionValue);
		break;
	case EOptionType::FirePenetration:
		OptionDescription = FString::Printf(TEXT("화염 관통력: %.1f"), OptionValue);
		break;
	case EOptionType::IcePenetration:
		OptionDescription = FString::Printf(TEXT("냉기 관통력: %.1f"), OptionValue);
		break;
	default:
		OptionDescription = TEXT("알 수 없는 옵션");
		break;
	}
}

FString UOptionDataAsset::GetOptionDescription()
{
	// OptionDescription 이 비어있으면 설정
	if (OptionDescription.IsEmpty())
	{
		SetOptionDescription();
	}
	return OptionDescription;
}

TArray<UOptionDataAsset*> UOptionDataAsset::GetRandomOptions(const TArray<UOptionDataAsset*>& OptionDataAssetArray,
                                                             const int32 ItemTier, const EItemRarity ItemRarity)
{
	TArray<UOptionDataAsset*> SelectedOptions;

	// Step 1: Filter options based on ItemTier
	TArray<UOptionDataAsset*> FilteredOptions;
	for (UOptionDataAsset* Option : OptionDataAssetArray)
	{
		if (Option && Option->OptionTier <= ItemTier)
		{
			FilteredOptions.Add(Option);
		}
	}

	// Early exit if rarity is Normal
	if (ItemRarity == EItemRarity::Normal)
	{
		return SelectedOptions; // Empty array
	}

	// Step 2: Determine the number of options based on ItemRarity
	int32 MinOptions = 0;
	int32 MaxOptions = 0;

	switch (ItemRarity)
	{
	case EItemRarity::Magic:
		MinOptions = 1;
		MaxOptions = 2;
		break;
	case EItemRarity::Rare:
		MinOptions = 3;
		MaxOptions = 4;
		break;
	default:
		// Handle unexpected rarity types
		return SelectedOptions;
	}

	// Randomly determine the number of options to select
	int32 NumOptionsToSelect = FMath::RandRange(MinOptions, MaxOptions);

	// Step 3: Weighted random selection without duplicate OptionType
	// We'll use a copy of FilteredOptions to modify during selection
	TArray<UOptionDataAsset*> AvailableOptions = FilteredOptions;
	TSet<EOptionType> SelectedOptionTypes;

	for (int32 i = 0; i < NumOptionsToSelect && AvailableOptions.Num() > 0; ++i)
	{
		// Calculate the total weight
		int32 TotalWeight = 0;
		for (UOptionDataAsset* Option : AvailableOptions)
		{
			TotalWeight += Option->OptionWeight;
		}

		if (TotalWeight <= 0)
		{
			break; // Cannot select any more options
		}

		// Generate a random number between 1 and TotalWeight
		int32 RandomWeight = FMath::RandRange(1, TotalWeight);
		int32 CumulativeWeight = 0;
		UOptionDataAsset* SelectedOption = nullptr;

		// Select the option based on the random weight
		for (int32 j = 0; j < AvailableOptions.Num(); ++j)
		{
			UOptionDataAsset* Option = AvailableOptions[j];
			CumulativeWeight += Option->OptionWeight;

			if (RandomWeight <= CumulativeWeight)
			{
				SelectedOption = Option;
				// Remove the selected option from AvailableOptions
				AvailableOptions.RemoveAt(j);
				break;
			}
		}

		if (SelectedOption)
		{
			// Add to the selected options
			SelectedOptions.Add(SelectedOption);
			// Track the OptionType to prevent duplicates
			SelectedOptionTypes.Add(SelectedOption->OptionType);

			// Remove all other options with the same OptionType
			AvailableOptions.RemoveAll([SelectedOption](UOptionDataAsset* Option)
			{
				return Option->OptionType == SelectedOption->OptionType;
			});
		}
	}

	return SelectedOptions;
}
