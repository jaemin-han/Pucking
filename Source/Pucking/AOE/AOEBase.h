// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AOEBase.generated.h"

UCLASS()
class PUCKING_API AAOEBase : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAOEBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bShouldLoop = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Radius = 1000;
};