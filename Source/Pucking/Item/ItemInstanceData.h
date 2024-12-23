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

	// ItemType이 Ammo인 경우 FAmmoData
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item", meta = (EditCondition = "ItemType == EItemType::Ammo"))
	FAmmoData AmmoData;
	
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
