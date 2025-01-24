// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyBase.h"
#include "World/PuckGameInstance.h"
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
	TArray<TSubclassOf<AEnemyBase>> EnemyClass;
	UPROPERTY(EditAnywhere)
	int32 PoolSize = 0;

	//EnemyVar
	UPROPERTY(EditAnywhere)
	int32 MinionIndex = 0;
	UPROPERTY(EditAnywhere)
	int32 TankIndex = 1;
	UPROPERTY(EditAnywhere)
	int32 RangerIndex = 2;

	UPROPERTY(EditAnywhere)
	UPuckGameInstance* PuckGameInstance;

	UPROPERTY(EditAnywhere)
	int32 NormalOrElite;
	UPROPERTY(EditAnywhere)
	int32 MinionOrTankOrRanger;
	UPROPERTY(EditAnywhere)
	float NormalWeight;
	UPROPERTY(EditAnywhere)
	float EliteWeight;
	UPROPERTY(EditAnywhere)
	float MinionWeight;
	UPROPERTY(EditAnywhere)
	float TankWeight;
	UPROPERTY(EditAnywhere)
	float RangerWeight;

	UPROPERTY(EditAnywhere)
	AEnemyBase* SpawnedEnemy;
	UPROPERTY(EditAnywhere)
	AEnemyObjectPool* EnemyPool;

	FTimerHandle SpawnHandle;

	float HalfTimeSeconds = 10;

	FTimerHandle HalfTimeHandle;

public:
	UFUNCTION(BlueprintCallable)
	FVector GetRandomSpawnLocation();

	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

	UFUNCTION(BlueprintCallable)
	void SpawnerInitialize();
	UFUNCTION(BlueprintCallable)
	void SpawnerReset();


	UFUNCTION(BlueprintCallable)
	void SpawnTimerStart();

	UFUNCTION(BlueprintCallable)
	void SpawnTimerClear();
	UFUNCTION(BlueprintCallable)
	void SetWeightByLevel();

	UFUNCTION(BlueprintCallable)
	void SettingNewEnemy();


};
