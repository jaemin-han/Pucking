// Fill out your copyright notice in the Description page of Project Settings.


#include "World/ObjectPoolTestEnemy.h"
#include "World/EnemyObjectPool.h"

AObjectPoolTestEnemy::AObjectPoolTestEnemy()
{
	bIsActive = false;
}

void AObjectPoolTestEnemy::Activate()
{
	bIsActive = true;
}

void AObjectPoolTestEnemy::Deactivate()
{
	bIsActive = false;
}

void AObjectPoolTestEnemy::Initialze(FVector SpawnLocation)
{
	SetActorLocation(SpawnLocation);
	Activate();
}

void AObjectPoolTestEnemy::Die()
{
	if (AEnemyObjectPool* Pool = GetWorld()->SpawnActor<AEnemyObjectPool>())
	{
		Pool->ReturnEnemy(this);
	}
}
