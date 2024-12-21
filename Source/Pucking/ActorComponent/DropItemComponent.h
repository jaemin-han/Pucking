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

	// DropItemTable
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DropItem")
	class UDataTable* DropItemTable;

	// Drop Item Actor Class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DropItem")
	TSubclassOf<class AItemBase> DropItemActorClass;

	// 아이템에 적용될 OptionDataAsset Array
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropItem")
	TArray<class UOptionDataAsset*> OptionDataAssets;

	// DropItemTable 에 있는 아이템을 랜덤하게 드랍
	void DropItem();
	// FItemDropData -> FItemInstanceData
	void SetItemInstanceData(const FItemDropData& ItemDropData, FItemInstanceData& ItemInstanceData);
};
