// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.h"
#include "World/PuckGameInstance.h"
#include "World/ObjectPoolTestEnemy.h"
#include "World/EnemyObjectPool.h"
#include "EnemySpawnerTest.generated.h"

UCLASS()
class PUCKING_API AEnemySpawnerTest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawnerTest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere)
	TArray<AActor*> SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AObjectPoolTestEnemy> EnemyClass;
	UPROPERTY(EditAnywhere)
	int32 PoolSize = 5;

	UPROPERTY(EditAnywhere)
	UPuckGameInstance* PuckGameInstance;

	UPROPERTY(EditAnywhere)
	int32 NormalOrElite;
	UPROPERTY(EditAnywhere)
	int32 MinionOrTankOrRanger;

	UPROPERTY(EditAnywhere)
	AEnemyObjectPool* EnemyPool;

public:
	UFUNCTION(BlueprintCallable)
	FVector GetRandomSpawnLocation();

	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();


};
