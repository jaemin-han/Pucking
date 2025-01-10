// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "PickableItem.generated.h"

UCLASS()
class PUCKING_API APickableItem : public AItemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	TSharedPtr<FPickableData> PickableData;
};
