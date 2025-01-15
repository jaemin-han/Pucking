// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemySpawnerTest.h"
#include "SpawnPoint.h"
#include "Engine/World.h"
#include "World/ObjectPoolTestEnemy.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemySpawnerTest::AEnemySpawnerTest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	


}

// Called when the game starts or when spawned
void AEnemySpawnerTest::BeginPlay()
{
	Super::BeginPlay();
	//GameInstance Casting
	PuckGameInstance = Cast<UPuckGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	//EnemyPool = Cast<AEnemyObjectPool>(UGameplayStatics::GetActorOfClass(this, AEnemyObjectPool::StaticClass()));
	EnemyPool = GetWorld()->SpawnActor<AEnemyObjectPool>();
	
	//SpawnPoints Find
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnPoint::StaticClass(), SpawnPoints);
	if (SpawnPoints.Num() > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("SpawnPoint Exist"));
	}

	PuckGameInstance->OnLevelChanged.AddDynamic(this, &AEnemySpawnerTest::SettingNewEnemy);

	SetWeightByLevel();
	SpawnerInitialize();
	SpawnTimerStart();
	
	
}

// Called every frame
void AEnemySpawnerTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AEnemySpawnerTest::GetRandomSpawnLocation()
{
	if (SpawnPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No SpawnPoint"));
		return FVector::ZeroVector;
	}

	int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);
	return SpawnPoints[RandomIndex]->GetActorLocation();
}

void AEnemySpawnerTest::SpawnEnemy()
{
	if (!EnemyClass)
	{
		return;
	}
	int32 NormalOrEliteRandom = FMath::RandRange(0, NormalOrElite - 1);
	int32 MinionOrTankOrRangerRandom = FMath::RandRange(0, MinionOrTankOrRanger - 1);
	FVector SpawnLocation = GetRandomSpawnLocation();
	FRotator SpawnRotator = FRotator::ZeroRotator;

	AObjectPoolTestEnemy* SpawnedEnemy = EnemyPool->GetEnemy();
	if (SpawnedEnemy)
	{
		if ((NormalOrEliteRandom < NormalWeight))
		{
			SpawnedEnemy->Initialze(SpawnLocation);
			//GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, SpawnRotator);
			UE_LOG(LogTemp, Warning, TEXT("Normal Spawn"))
		}
		else if ((NormalOrEliteRandom > NormalWeight))
		{


			SpawnedEnemy->Initialze(SpawnLocation);
			//GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, SpawnRotator);
			UE_LOG(LogTemp, Warning, TEXT("Elite Spawn"))

		}
	}

	
	

}

void AEnemySpawnerTest::SpawnerInitialize()
{
	if (EnemyPool)
	{
		EnemyPool->InitializePool(PoolSize, EnemyClass);
	}
}

void AEnemySpawnerTest::SpawnerReset()
{
	if (EnemyPool)
	{
		EnemyPool->ResetPool();
	}
}

void AEnemySpawnerTest::SpawnTimerStart()
{
	GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &AEnemySpawnerTest::SpawnEnemy, 2, true);
}

void AEnemySpawnerTest::SpawnTimerClear()
{
	GetWorld()->GetTimerManager().ClearTimer(SpawnHandle);
}

void AEnemySpawnerTest::SetWeightByLevel()
{
	NormalWeight = PuckGameInstance->CurrentRow.NormalWeight;
	EliteWeight = PuckGameInstance->CurrentRow.EliteWeight;
	MinionWeight = PuckGameInstance->CurrentRow.MinionWeight;
	TankWeight = PuckGameInstance->CurrentRow.TankWeight;
	RangerWeight = PuckGameInstance->CurrentRow.RangerWeight;

	NormalOrElite = NormalWeight + EliteWeight;
	MinionOrTankOrRanger = MinionWeight + TankWeight + RangerWeight;
}

void AEnemySpawnerTest::SettingNewEnemy()
{
	SetWeightByLevel();
	SpawnTimerClear();
	SpawnerReset();
	SpawnerInitialize();
	SpawnTimerStart();
}


