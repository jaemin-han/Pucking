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

	if (EnemyPool)
	{
		EnemyPool->InitializePool(30, EnemyClass);
	}

	FTimerHandle SpawnHandle;
	GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &AEnemySpawnerTest::SpawnEnemy, 2, true);
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
	NormalOrElite = PuckGameInstance->CurrentRow.NormalWeight + PuckGameInstance->CurrentRow.EliteWeight;
	MinionOrTankOrRanger = PuckGameInstance->CurrentRow.MinionWeight + PuckGameInstance->CurrentRow.TankWeight + PuckGameInstance->CurrentRow.RangerWeight;
	int32 NormalOrEliteRandom = FMath::RandRange(0, NormalOrElite - 1);
	int32 MinionOrTankOrRangerRandom = FMath::RandRange(0, MinionOrTankOrRanger - 1);
	FVector SpawnLocation = GetRandomSpawnLocation();
	FRotator SpawnRotator = FRotator::ZeroRotator;

	AObjectPoolTestEnemy* SpawnedEnemy = EnemyPool->GetEnemy();
	if (SpawnedEnemy)
	{
		if ((NormalOrEliteRandom < PuckGameInstance->CurrentRow.NormalWeight))
		{
			SpawnedEnemy->Initialze(SpawnLocation);
			//GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, SpawnRotator);
			UE_LOG(LogTemp, Warning, TEXT("Normal Spawn"))
		}
		else if ((NormalOrEliteRandom > PuckGameInstance->CurrentRow.NormalWeight))
		{


			SpawnedEnemy->Initialze(SpawnLocation);
			//GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, SpawnRotator);
			UE_LOG(LogTemp, Warning, TEXT("Elite Spawn"))

		}
	}

	
	

}


