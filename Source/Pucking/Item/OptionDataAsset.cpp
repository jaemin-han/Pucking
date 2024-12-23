// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionDataAsset.h"

void UOptionDataAsset::SetOptionDescription()
{
	// OptionType, OptionValue에 따라 OptionDescription을 설정
	// 예시: OptionType 이 EOptionType::Critical, OptionValue가 0.5일 경우 "치명타 0.5"로 설정
	switch (OptionType)
	{
	case EOptionType::Critical:
		OptionDescription = FString::Printf(TEXT("치명타 %.1f"), OptionValue);
		break;
	case EOptionType::Dmg:
	    OptionDescription = FString::Printf(TEXT("데미지 %.1f"), OptionValue);
		break;
	case EOptionType::DF:
			    OptionDescription = FString::Printf(TEXT("방어력 %.1f"), OptionValue);
		break;
	case EOptionType::MaxHP:
			    OptionDescription = FString::Printf(TEXT("최대 체력 %.1f"), OptionValue);
		break;
	default:
		OptionDescription = FString::Printf(TEXT("Unknown Option"));
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
            AvailableOptions.RemoveAll([SelectedOption](UOptionDataAsset* Option) {
                return Option->OptionType == SelectedOption->OptionType;
            });
        }
    }

    return SelectedOptions;
}

