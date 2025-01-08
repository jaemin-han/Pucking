// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "OverlapItem.generated.h"

UCLASS()
class PUCKING_API AOverlapItem : public AItemBase
{
	GENERATED_BODY()

protected:
	

public:
	// Sets default values for this actor's properties
	AOverlapItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	TSharedPtr<FOverlapData> OverlapData;
	FTimerHandle CheckCollisionTimerHandle;

protected:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                    int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	void CheckCollision();

public:
	void OnInitialize();
};
