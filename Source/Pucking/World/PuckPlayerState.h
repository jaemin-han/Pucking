// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PuckPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API APuckPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Essence")
	int32 Essence;

protected:
	virtual void Tick(float DeltaSeconds) override;
	

public:
	APuckPlayerState();
	int32 GetEssence() const { return Essence; }
	void SetEssence(const int32 NewEssence) { Essence = NewEssence; }
	void AddEssence(const int32 AddEssence) { Essence += AddEssence; }
};
