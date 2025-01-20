// Fill out your copyright notice in the Description page of Project Settings.


#include "World/EnemyObjectPool.h"

// Sets default values
AEnemyObjectPool::AEnemyObjectPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AEnemyObjectPool::InitializePool(int32 InPoolSize, TSubclassOf<AEnemyBase> InEnemyClass, int32 EnemyIndex)
{
	//MinionInit
	if (EnemyIndex == 0)
	{
		PoolSize = InPoolSize;
		MinionEnemyClass = InEnemyClass;

		if (!MinionEnemyClass)
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
			AEnemyBase* Enemy = World->SpawnActor<AEnemyBase>(MinionEnemyClass);
			if (Enemy)
			{
				Enemy->SetActorEnableCollision(false);
				Enemy->SetActorHiddenInGame(true);
				//Enemy->ReturnAfterDelay();
				MinionPool.Add(Enemy);
				UE_LOG(LogTemp, Warning, TEXT("ObjectPool::InitializePool"));
			}
		}
	}
	//TankInit
	else if (EnemyIndex == 1)
	{
		PoolSize = InPoolSize;
		TankEnemyClass = InEnemyClass;

		if (!TankEnemyClass)
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
			AEnemyBase* Enemy = World->SpawnActor<AEnemyBase>(TankEnemyClass);
			if (Enemy)
			{
				Enemy->SetActorEnableCollision(false);
				Enemy->SetActorHiddenInGame(true);
				//Enemy->ReturnAfterDelay();
				TankPool.Add(Enemy);
				UE_LOG(LogTemp, Warning, TEXT("ObjectPool::InitializePool"));
			}
		}
	}
	//RangerInit
	else if (EnemyIndex == 2)
	{
		PoolSize = InPoolSize;
		RangerEnemyClass = InEnemyClass;

		if (!RangerEnemyClass)
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
			AEnemyBase* Enemy = World->SpawnActor<AEnemyBase>(RangerEnemyClass);
			if (Enemy)
			{
				Enemy->SetActorEnableCollision(false);
				Enemy->SetActorHiddenInGame(true);
				//Enemy->ReturnAfterDelay();
				RangerPool.Add(Enemy);
				UE_LOG(LogTemp, Warning, TEXT("ObjectPool::InitializePool"));
			}
		}
	}
	
	
}

//If Stage Level Up->Enemy's NewStatus Init
void AEnemyObjectPool::ResetPool()
{
	for (AEnemyBase* Enemy : MinionPool)
	{
		if (Enemy && !Enemy->IsActive())
		{
			Enemy->EnemyBaseStatusInit();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Minion Empty"))
		}
	}
	//MinionPool.Reset();
	for (AEnemyBase* Enemy : TankPool)
	{
		if (Enemy && !Enemy->IsActive())
		{
			Enemy->EnemyBaseStatusInit();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Tank Empty"))
		}
	}
	//TankPool.Reset();
	for (AEnemyBase* Enemy : RangerPool)
	{
		if (Enemy && !Enemy->IsActive())
		{
			Enemy->EnemyBaseStatusInit();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Ranger Empty"))
		}
	}
	//RangerPool.Reset();

	UE_LOG(LogTemp, Warning, TEXT("ObjectPool::ResetPool"));
}

AEnemyBase* AEnemyObjectPool::GetEnemy(int32 EnemyIndex)
{
	//GetMinion
	if (EnemyIndex == 0)
	{
		for (AEnemyBase* Enemy : MinionPool)
		{
			if (Enemy && !Enemy->IsActive())
			{
				Enemy->SetActorEnableCollision(true);
				Enemy->SetActorHiddenInGame(false);
				Enemy->Revive(); // 활성화 메서드
				UE_LOG(LogTemp, Warning, TEXT("ObjectPool::GetEnemy::RecycleEnemy"));
				return Enemy;
			}
		}
		if (MinionEnemyClass)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				AEnemyBase* NewEnemy = World->SpawnActor<AEnemyBase>(MinionEnemyClass);
				if (NewEnemy)
				{
					MinionPool.Add(NewEnemy);
					NewEnemy->SetActorEnableCollision(true);
					NewEnemy->SetActorHiddenInGame(false);
					NewEnemy->Revive(); // 활성화 메서드
					UE_LOG(LogTemp, Warning, TEXT("ObjectPool::GetEnemy::SpawnNewEnemy"));
					return NewEnemy;
				}
			}
		}
	}
	//GetTank
	else if (EnemyIndex == 1)
	{
		for (AEnemyBase* Enemy : TankPool)
		{
			if (Enemy && !Enemy->IsActive())
			{
				Enemy->SetActorEnableCollision(true);
				Enemy->SetActorHiddenInGame(false);
				Enemy->Revive(); // 활성화 메서드
				UE_LOG(LogTemp, Warning, TEXT("ObjectPool::GetEnemy::RecycleEnemy"));
				return Enemy;
			}
		}
		if (TankEnemyClass)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				AEnemyBase* NewEnemy = World->SpawnActor<AEnemyBase>(TankEnemyClass);
				if (NewEnemy)
				{
					TankPool.Add(NewEnemy);
					NewEnemy->SetActorEnableCollision(true);
					NewEnemy->SetActorHiddenInGame(false);
					NewEnemy->Revive(); // 활성화 메서드
					UE_LOG(LogTemp, Warning, TEXT("ObjectPool::GetEnemy::SpawnNewEnemy"));
					return NewEnemy;
				}
			}
		}
	}
	//GetRanger
	else if (EnemyIndex == 2)
	{
		for (AEnemyBase* Enemy : RangerPool)
		{
			if (Enemy && !Enemy->IsActive())
			{
				Enemy->SetActorEnableCollision(true);
				Enemy->SetActorHiddenInGame(false);
				Enemy->Revive(); // 활성화 메서드
				UE_LOG(LogTemp, Warning, TEXT("ObjectPool::GetEnemy::RecycleEnemy"));
				return Enemy;
			}
		}
		if (RangerEnemyClass)
		{
			UWorld* World = GetWorld();
			if (World)
			{
				AEnemyBase* NewEnemy = World->SpawnActor<AEnemyBase>(RangerEnemyClass);
				if (NewEnemy)
				{
					RangerPool.Add(NewEnemy);
					NewEnemy->SetActorEnableCollision(true);
					NewEnemy->SetActorHiddenInGame(false);
					NewEnemy->Revive(); // 활성화 메서드
					UE_LOG(LogTemp, Warning, TEXT("ObjectPool::GetEnemy::SpawnNewEnemy"));
					return NewEnemy;
				}
			}
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("No available enemies in the pool!"));
	return nullptr;
}

void AEnemyObjectPool::ReturnEnemy(AEnemyBase* Enemy)
{
	if (Enemy)
	{
		//Enemy->SetActorEnableCollision(false);
		//Enemy->SetActorHiddenInGame(true);
		Enemy->Die(); // 비활성화 메서드
		UE_LOG(LogTemp, Warning, TEXT("ObjectPool::ReturnEnemy"));
	}
}

// Called when the game starts or when spawned
void AEnemyObjectPool::BeginPlay()
{
	Super::BeginPlay();
	
}


