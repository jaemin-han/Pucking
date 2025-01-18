// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DropItemComponent.generated.h"


struct FItemInstanceData;
struct FItemDropData;

UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PUCKING_API UDropItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UDropItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void DestroyComponent(bool bPromoteChildren = false) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	// item tier: Owner 가 가지고 있는 ItemTier
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DropItem")
	int32 ItemTier;

	// Owner 가 가지고 있는 Item Rarity Multiplier
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DropItem")
	float ItemRarityMultiplier;

	// 드랍율 배율
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DropItem")
	float DropRateMultiplier;

	// DropItemTable
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DropItem")
	class UDataTable* DropItemTable;

	// Drop Item Actor Class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DropItem")
	TSubclassOf<class APickableItem> DropItemActorClass;

	// OverlapItem Actor Class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DropItem")
	TSubclassOf<class AOverlapItem> OverlapItemActorClass;

	// 아이템에 적용될 OptionDataAsset Array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropItem")
	TArray<class UOptionDataAsset*> OptionDataAssets;

	// DropItemTable 에 있는 아이템을 랜덤하게 드랍
	UFUNCTION(BlueprintCallable, Category = "DropItem")
	void DropItem();
	// FItemDropData -> FItemInstanceData
	void SetItemInstanceData(const FItemDropData& ItemDropData, FItemInstanceData& ItemInstanceData);

	// setter
	void SetItemTier(int32 NewItemTier) { ItemTier = NewItemTier; }
	void SetItemRarityMultiplier(float NewItemRarityMultiplier) { ItemRarityMultiplier = NewItemRarityMultiplier; }
	void SetDropRateMultiplier(float NewDropRateMultiplier) { DropRateMultiplier = NewDropRateMultiplier; }
};
