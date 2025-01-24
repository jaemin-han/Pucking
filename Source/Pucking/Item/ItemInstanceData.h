// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemDropData.h"
#include "Common/CommonEnum.h"
#include "ItemInstanceData.generated.h"


struct FOption;

USTRUCT(BlueprintType)
struct FItemInstanceData : public FTableRowBase
{
	GENERATED_BODY()

	// Constructor
	FItemInstanceData()
	{
		ItemName = FText::FromString("Default Item Name");
		ItemStaticMesh = nullptr;
		ItemSkeletalMesh = nullptr;
		ItemType = EItemType::Empty;
		ItemRarity = EItemRarity::Normal;
		ItemThumbnail = nullptr;
		bStackable = false;
		MaxStackCount = 1;
		ItemOptions.Empty();
		ItemOptionDescription = FString();
	}
	
	// item name
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FText ItemName;

	// static mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UStaticMesh* ItemStaticMesh;

	// skeletal mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class USkeletalMesh* ItemSkeletalMesh;

	// item type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType;
	
	// item rarity
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemRarity ItemRarity;

	// item thumbnail
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	class UTexture2D* ItemThumbnail;

	// stackable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	bool bStackable;

	// max stack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (EditCondition = "bStackable"))
	int32 MaxStackCount;

	// Item Option Array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<class UOptionDataAsset*> ItemOptions;

	// Original Item Options, Item Popup 에서 사용함
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TArray<class UOptionDataAsset*> OriginalItemOptions;

	// Item Option Description, ItemOptions 에 있는 OptionDataAsset 들의 OptionDescription 을 모두 합친 것
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FString ItemOptionDescription;
};

/**
 * 
 */
class PUCKING_API ItemInstanceData
{
public:
	ItemInstanceData();
	~ItemInstanceData();
};
