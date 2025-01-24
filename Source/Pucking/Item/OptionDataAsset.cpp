// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionDataAsset.h"

FString UOptionDataAsset::GetDamageTypeDescription()
{
	FString Description;
	switch (static_cast<EDamageType>(OptionValue))
	{
	case EDamageType::Physical:
		Description = TEXT("Physical");
		break;
	case EDamageType::Fire:
		Description = TEXT("Fire");
		break;
	case EDamageType::Ice:
		Description = TEXT("Ice");
		break;
	default:
		Description = TEXT("Unknown Damage Type");
		break;
	}
	return Description;
}

void UOptionDataAsset::SetOptionDescription()
{
	switch (OptionType)
	{
	case EOptionType::DamageType:
		OptionDescription = FString::Printf(TEXT("Damage Type: %s"), *GetDamageTypeDescription());
		break;
	case EOptionType::Damage:
		OptionDescription = FString::Printf(TEXT("Damage: %.1f"), OptionValue);
		break;
	case EOptionType::CriticalRate:
		OptionDescription = FString::Printf(TEXT("Critical Rate: %.1f%%"), OptionValue);
		break;
	case EOptionType::CriticalMultiplier:
		OptionDescription = FString::Printf(TEXT("Critical Multiplier: %.1f"), OptionValue);
		break;
	case EOptionType::PhysicalPenetration:
		OptionDescription = FString::Printf(TEXT("Physical Penetration: %.1f"), OptionValue);
		break;
	case EOptionType::FirePenetration:
		OptionDescription = FString::Printf(TEXT("Fire Penetration: %.1f"), OptionValue);
		break;
	case EOptionType::IcePenetration:
		OptionDescription = FString::Printf(TEXT("Ice Penetration: %.1f"), OptionValue);
		break;
	case EOptionType::StaggerValue:
		OptionDescription = FString::Printf(TEXT("Stagger Value: %.1f"), OptionValue);
		break;
	default:
		OptionDescription = TEXT("Unknown Option Type");
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

FString UOptionDataAsset::GetOptionString()
{
	switch (OptionType)
	{
	case EOptionType::DamageType:
		return TEXT("Damage Type: ");
	case EOptionType::Damage:
		return TEXT("Damage: ");
	case EOptionType::CriticalRate:
		return TEXT("Critical Rate: ");
	case EOptionType::CriticalMultiplier:
		return TEXT("Critical Multiplier: ");
	case EOptionType::PhysicalPenetration:
		return TEXT("Physical Penetration: ");
	case EOptionType::FirePenetration:
		return TEXT("Fire Penetration: ");
	case EOptionType::IcePenetration:
		return TEXT("Ice Penetration: ");
	case EOptionType::StaggerValue:
		return TEXT("Stagger Value: ");
	default:
		return TEXT("Unknown Option Type: ");
	}
}

TArray<UOptionDataAsset*> UOptionDataAsset::GetRandomOptions(const TArray<UOptionDataAsset*>& OptionDataAssetArray,
                                                             const int32 MinItemTier, const int32 MaxItemTier,
                                                             const EItemRarity ItemRarity)
{
	TArray<UOptionDataAsset*> SelectedOptions;

	// Step 1: Filter options based on ItemTier range
	TArray<UOptionDataAsset*> FilteredOptions;
	for (UOptionDataAsset* Option : OptionDataAssetArray)
	{
		if (Option && Option->OptionTier >= MinItemTier && Option->OptionTier <= MaxItemTier)
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
