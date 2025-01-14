// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy/EnemyBase.h"
#include "ObjectPoolTestEnemy.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API AObjectPoolTestEnemy : public AEnemyBase
{
	GENERATED_BODY()
		
public:
	AObjectPoolTestEnemy();

	void Activate();
	void Deactivate();

	bool IsActive() const {
		return bIsActive;
	}

	void Initialze(FVector SpawnLocation);

	void Die();

private:
	bool bIsActive;
};
