// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Enemy/EnemyBase.h"
#include "EnemyObjectPool.generated.h"

UCLASS()
class PUCKING_API AEnemyObjectPool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyObjectPool();

	void InitializePool(int32 PoolSize, TSubclassOf<AEnemyBase> EnemyClass, int32 EnemyIndex);

	void ResetPool();

	AEnemyBase* GetEnemy(int32 EnemyIndex);

	void ReturnEnemy(AEnemyBase* Enemy);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEnemyBase> MinionEnemyClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEnemyBase> TankEnemyClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEnemyBase> RangerEnemyClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AEnemyBase*> MinionPool;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AEnemyBase*> TankPool;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AEnemyBase*> RangerPool;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PoolSize;
};
