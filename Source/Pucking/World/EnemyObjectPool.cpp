// Fill out your copyright notice in the Description page of Project Settings.


#include "World/EnemyObjectPool.h"

// Sets default values
AEnemyObjectPool::AEnemyObjectPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AEnemyObjectPool::InitializePool(int32 InPoolSize, TSubclassOf<AObjectPoolTestEnemy> InEnemyClass)
{
	PoolSize = InPoolSize;
	EnemyClass = InEnemyClass;

	if (!EnemyClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyClass is not set!"));
		return;
	}

	UWorld* World = GetWorld();
	if (!World)
	{
		UE_LOG(LogTemp, Warning, TEXT("World is Null"));
		return;
	}

	for (int32 i = 0; i < PoolSize; i++)
	{
		AObjectPoolTestEnemy* Enemy = World->SpawnActor<AObjectPoolTestEnemy>(EnemyClass);
		if (Enemy)
		{
			Enemy->SetActorEnableCollision(false);
			Enemy->SetActorHiddenInGame(true);
			EnemyPool.Add(Enemy);
		}
	}
}

AObjectPoolTestEnemy* AEnemyObjectPool::GetEnemy()
{
	for (AObjectPoolTestEnemy* Enemy : EnemyPool)
	{
		if (Enemy && !Enemy->IsActive())
		{
			Enemy->SetActorEnableCollision(true);
			Enemy->SetActorHiddenInGame(false);
			Enemy->Activate(); // 활성화 메서드
			return Enemy;
		}
	}
	if (EnemyClass)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			AObjectPoolTestEnemy* NewEnemy = World->SpawnActor<AObjectPoolTestEnemy>(EnemyClass);
			if (NewEnemy)
			{
				EnemyPool.Add(NewEnemy);
				NewEnemy->SetActorEnableCollision(true);
				NewEnemy->SetActorHiddenInGame(false);
				NewEnemy->Activate(); // 활성화 메서드
				return NewEnemy;
			}
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("No available enemies in the pool!"));
	return nullptr;
}

void AEnemyObjectPool::ReturnEnemy(AObjectPoolTestEnemy* Enemy)
{
	if (Enemy)
	{
		Enemy->SetActorEnableCollision(false);
		Enemy->SetActorHiddenInGame(true);
		Enemy->Deactivate(); // 비활성화 메서드
	}
}

// Called when the game starts or when spawned
void AEnemyObjectPool::BeginPlay()
{
	Super::BeginPlay();
	
}


