// Fill out your copyright notice in the Description page of Project Settings.


#include "DropItemComponent.h"

#include "Item/ItemBase.h"
#include "Item/ItemDropData.h"


// Sets default values for this component's properties
UDropItemComponent::UDropItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDropItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UDropItemComponent::DestroyComponent(bool bPromoteChildren)
{
	Super::DestroyComponent(bPromoteChildren);
	// debug
	UE_LOG(LogTemp, Warning, TEXT("DropItemComponent Destroyed"));
}

void UDropItemComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	// editor 가 꺼졌을 때는 DropItem 을 수행하지 않음
	if (EndPlayReason == EEndPlayReason::Destroyed)
	{
		UE_LOG(LogTemp, Warning, TEXT("DropItemComponent EndPlay"));
		DropItem();
	}
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
		if (ItemDropData)
		{
			float RandomValue = FMath::FRandRange(0.f, 1.f);
			if (RandomValue <= ItemDropData->ItemDropRate)
			{
				FItemInstanceData ItemInstanceData;
				ItemInstanceData.ItemName = ItemDropData->ItemName;
				ItemInstanceData.ItemStaticMesh = ItemDropData->ItemStaticMesh;
				ItemInstanceData.ItemSkeletalMesh = ItemDropData->ItemSkeletalMesh;
				ItemInstanceData.ItemType = ItemDropData->ItemType;
				ItemInstanceData.ItemThumbnail = ItemDropData->ItemThumbnail;
				ItemInstanceData.bStackable = ItemDropData->bStackable;
				ItemInstanceData.MaxStackCount = ItemDropData->MaxStackCount;
				// todo: ItemRarity selection
				ItemInstanceData.ItemRarity = EItemRarity::Normal;
				// todo: ItemOptions


				auto* DropItem = GetWorld()->SpawnActor<AItemBase>(DropItemActorClass, GetOwner()->GetActorLocation(),
				                                                   FRotator::ZeroRotator);
				DropItem->ItemData = ItemInstanceData;
				DropItem->ConstructMesh();
			}
		}
	}
}
