// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "PickableItem.generated.h"

UCLASS()
class PUCKING_API APickableItem : public AItemBase
{
	GENERATED_BODY()

	// StaticMeshComponent - LightBeam
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* LightBeam;

public:
	// Sets default values for this actor's properties
	APickableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void ConstructMesh() const override;
	virtual void SetItemData(const struct FItemDropData& ItemDropData) override;

public:
	TSharedPtr<FPickableData> PickableData;

};
