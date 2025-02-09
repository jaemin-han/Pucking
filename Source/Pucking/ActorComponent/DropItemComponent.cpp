// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItemComponent.h"

#include "Item/ItemDropData.h"
#include "Item/OptionDataAsset.h"
#include "Item/OverlapItem.h"
#include "Item/PickableItem.h"


class APuckGameState;
// Sets default values for this component's properties
UDropItemComponent::UDropItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// todo: enemy 가 스폰될 때 적절한 ItemTier, DropRateMultiplier 를 설정해야 함
	ItemTier = 1;
	DropRateMultiplier = 100.f;
	ItemRarityMultiplier = 100.f;
	// ...
}


// Called when the game starts
void UDropItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UDropItemComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDropItemComponent::DropItem()
{
	// DropItemTable 을 Row 별로 순회하면서 아이템을 랜덤하게 드랍
	TArray<FName> RowNames = DropItemTable->GetRowNames();
	for (FName RowName : RowNames)
	{
		FItemDropData* ItemDropData = DropItemTable->FindRow<FItemDropData>(RowName, TEXT(""));
		if (!ItemDropData)
			break;

		// DropItemComponent 의 ItemTier 가 ItemDropData 의 ItemTier 보다 낮으면 드랍하지 않음
		if (ItemTier < ItemDropData->ItemTier)
			continue;

		// DropRateMultiplier 를 곱한 값보다 RandomValue 가 크면 드랍하지 않음
		const float RandomValue = FMath::FRandRange(0.f, 1.f);
		const float AdjustedDropRate = ItemDropData->ItemDropRate * (DropRateMultiplier / 100.0f);

		if (RandomValue > AdjustedDropRate)
			continue;

		// ItemType 에 따라 드랍 로직 변경
		if (ItemDropData->ItemType == EItemType::Ammo)
		{
			auto* DropAmmo = GetWorld()->SpawnActor<APickableItem>(DropItemActorClass, GetOwner()->GetActorLocation(),
			                                                       FRotator::ZeroRotator);

			DropAmmo->ItemData.ItemRarity = GetItemRarity(*ItemDropData);
			DropAmmo->SetItemData(*ItemDropData);
			SetItemRarityAndOptions(*ItemDropData, DropAmmo->ItemData);
			// DropAmmo->ConstructMesh();

			// PostInitialize
			DropAmmo->PostInitialize();
		}
		else if (ItemDropData->ItemType == EItemType::Essence)
		{
			auto* DropEssence = GetWorld()->SpawnActor<AOverlapItem>(OverlapItemActorClass,
			                                                         GetOwner()->GetActorLocation(),
			                                                         FRotator::ZeroRotator);
			DropEssence->SetItemData(*ItemDropData);
		}
		else if (ItemDropData->ItemType == EItemType::HealthMarble)
		{
			auto* DropHealthMarble = GetWorld()->SpawnActor<AOverlapItem>(OverlapItemActorClass,
			                                                              GetOwner()->GetActorLocation(),
			                                                              FRotator::ZeroRotator);
			DropHealthMarble->SetItemData(*ItemDropData);
		}
	}
}

void UDropItemComponent::SetItemRarityAndOptions(const FItemDropData& ItemDropData, FItemInstanceData& ItemInstanceData)
{
	if (ItemDropData.ItemType != EItemType::Ammo)
		return;

	// ItemDropData 의 RarityRate 에 따라 ItemInstanceData 의 ItemRarity 를 설정
	// const float TotalMultiplier = ItemDropData.NormalWeight + ItemDropData.MagicWeight * (ItemRarityMultiplier / 100.0f)
	// 	+ ItemDropData.RareWeight * (ItemRarityMultiplier / 100.0f);
	// const float RandomValue = FMath::FRandRange(0.f, TotalMultiplier);
	// if (RandomValue <= ItemDropData.NormalWeight)
	// {
	// 	ItemInstanceData.ItemRarity = EItemRarity::Normal;
	// }
	// else if (RandomValue <= ItemDropData.NormalWeight + ItemDropData.MagicWeight)
	// {
	// 	ItemInstanceData.ItemRarity = EItemRarity::Magic;
	// }
	// else
	// {
	// 	ItemInstanceData.ItemRarity = EItemRarity::Rare;
	// }


	// todo: ItemOptions
	// todo: 아이템 type 에 따라 다른 Option Table or DataAsset 을 사용해게 수정할 가능성이 있음
	ItemInstanceData.ItemOptions = UOptionDataAsset::GetRandomOptions(OptionDataAssets, 1, ItemTier,
	                                                                  ItemInstanceData.ItemRarity);

	// copy ItemOptions to OriginalItemOptions
	ItemInstanceData.OriginalItemOptions = ItemInstanceData.ItemOptions;

	// ItemType 이 Ammo 일 경우, DamageType 과 AmmoDamage, CriticalRate, CriticalMultiplier
	// 각각의 옵션을 추가로 설정

	if (ItemInstanceData.ItemType == EItemType::Ammo)
	{
		auto& AmmoData = ItemDropData.AmmoData;

		// damage type
		UOptionDataAsset* AmmoDamageTypeOption = NewObject<UOptionDataAsset>();
		AmmoDamageTypeOption->SetOptionType(EOptionType::DamageType);
		AmmoDamageTypeOption->SetOptionValue(static_cast<float>(AmmoData.DamageType));

		// damage 
		UOptionDataAsset* AmmoDamageOption = NewObject<UOptionDataAsset>();
		AmmoDamageOption->SetOptionType(EOptionType::Damage);
		AmmoDamageOption->SetOptionValue(AmmoData.AmmoDamage);

		// critical rate
		UOptionDataAsset* AmmoCriticalRateOption = NewObject<UOptionDataAsset>();
		AmmoCriticalRateOption->SetOptionType(EOptionType::CriticalRate);
		AmmoCriticalRateOption->SetOptionValue(AmmoData.CriticalRate);

		// critical multiplier
		UOptionDataAsset* AmmoCriticalMultiplierOption = NewObject<UOptionDataAsset>();
		AmmoCriticalMultiplierOption->SetOptionType(EOptionType::CriticalMultiplier);
		AmmoCriticalMultiplierOption->SetOptionValue(AmmoData.CriticalMultiplier);

		// Stagger Value
		UOptionDataAsset* AmmoStaggerValueOption = NewObject<UOptionDataAsset>();
		AmmoStaggerValueOption->SetOptionType(EOptionType::StaggerValue);
		AmmoStaggerValueOption->SetOptionValue(AmmoData.StaggerValue);

		// add all options to ItemInstanceData.ItemOptions
		ItemInstanceData.ItemOptions.Add(AmmoDamageTypeOption);
		ItemInstanceData.ItemOptions.Add(AmmoDamageOption);
		ItemInstanceData.ItemOptions.Add(AmmoCriticalRateOption);
		ItemInstanceData.ItemOptions.Add(AmmoCriticalMultiplierOption);
		ItemInstanceData.ItemOptions.Add(AmmoStaggerValueOption);
	}

	// ItemInstanceData.ItemOptions 에 있는 OptionDataAsset 들의 OptionDescription 을 모두 합친 것
	for (UOptionDataAsset* OptionDataAsset : ItemInstanceData.ItemOptions)
	{
		ItemInstanceData.ItemOptionDescription += OptionDataAsset->GetOptionDescription() + TEXT("\n");
	}
}

EItemRarity UDropItemComponent::GetItemRarity(const FItemDropData& ItemDropData)
{
	// ItemDropData 의 RarityRate 에 따라 ItemInstanceData 의 ItemRarity 를 설정
	const float TotalMultiplier = ItemDropData.NormalWeight + ItemDropData.MagicWeight * (ItemRarityMultiplier / 100.0f)
		+ ItemDropData.RareWeight * (ItemRarityMultiplier / 100.0f);
	const float RandomValue = FMath::FRandRange(0.f, TotalMultiplier);
	if (RandomValue <= ItemDropData.NormalWeight)
	{
		return EItemRarity::Normal;
	}
	else if (RandomValue <= ItemDropData.NormalWeight + ItemDropData.MagicWeight)
	{
		return EItemRarity::Magic;
	}
	else
	{
		return EItemRarity::Rare;
	}
}
