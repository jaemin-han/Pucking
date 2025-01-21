// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemySpawnerTest.h"
#include "SpawnPoint.h"
#include "Engine/World.h"
#include "World/ObjectPoolTestEnemy.h"
#include "Kismet/GameplayStatics.h"

// todo: 재민
#include "ActorComponent/DropItemComponent.h"
#include "ActorComponent/StatusComponent.h"
#include "Item/OptionDataAsset.h"

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
	if (EnemyClass.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemySpawnerTest::SpawnEnemy"));
		return;
	}
	int32 NormalOrEliteRandom = FMath::RandRange(0, NormalOrElite - 1);
	int32 MinionOrTankOrRangerRandom = FMath::RandRange(0, MinionOrTankOrRanger);
	FVector SpawnLocation = GetRandomSpawnLocation();
	FRotator SpawnRotator = FRotator::ZeroRotator;

	if (MinionOrTankOrRangerRandom <= MinionWeight)
	{
		SpawnedEnemy = EnemyPool->GetEnemy(MinionIndex);
	}
	else if ((MinionOrTankOrRangerRandom > MinionWeight) && (MinionOrTankOrRangerRandom <= TankWeight + MinionWeight))
	{
		SpawnedEnemy = EnemyPool->GetEnemy(TankIndex);

	}
	else if ((MinionOrTankOrRangerRandom > TankWeight + MinionWeight) && (MinionOrTankOrRangerRandom <= MinionOrTankOrRanger))
	{
		SpawnedEnemy = EnemyPool->GetEnemy(RangerIndex);

	}
	if (SpawnedEnemy)
	{
		// todo: 재민 - 몬스터의 DropComponent 설정
		auto* DropItemComponent = SpawnedEnemy->FindComponentByClass<UDropItemComponent>();
		if (!DropItemComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("DropItemComponent is not found"));
		}

		if ((NormalOrEliteRandom < NormalWeight))
		{
			//GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, SpawnRotator);
			UE_LOG(LogTemp, Warning, TEXT("Normal Spawn"))
			// todo: 재민 - Normal 옵션 적용
			if (DropItemComponent)
			{
				DropItemComponent->SetItemTier(PuckGameInstance->CurrentRow.ItemTier);
				DropItemComponent->SetItemRarityMultiplier(PuckGameInstance->CurrentRow.NormalRarityMultiplier);
				DropItemComponent->SetDropRateMultiplier(PuckGameInstance->CurrentRow.NormalEnemyDropMultiplier);
			}
			SpawnedEnemy->Initialize(SpawnLocation);
		}
		else if ((NormalOrEliteRandom > NormalWeight))
		{


			//GetWorld()->SpawnActor<AActor>(EnemyClass, SpawnLocation, SpawnRotator);
			UE_LOG(LogTemp, Warning, TEXT("Elite Spawn"))
			// todo: 재민 - Elite 옵션 적용
			auto* StatusComponent = SpawnedEnemy->FindComponentByClass<UStatusComponent>();
			if (StatusComponent)
			{
				TArray<UOptionDataAsset*> DataAssets = UOptionDataAsset::GetRandomOptions(
					PuckGameInstance->EnemyOptionDataAssets, 1, PuckGameInstance->CurrentRow.EnemyOptionTier,
					EItemRarity::Magic);
				UE_LOG(LogTemp, Warning, TEXT("DataAssets Num : %d"), DataAssets.Num());
				StatusComponent->ApplyOptionByDataAssets(DataAssets);
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("StatusComponent is not found"));
			}
			if (DropItemComponent)
			{
				DropItemComponent->SetItemTier(PuckGameInstance->CurrentRow.ItemTier);
				DropItemComponent->SetItemRarityMultiplier(PuckGameInstance->CurrentRow.EliteRarityMultiplier);
				DropItemComponent->SetDropRateMultiplier(PuckGameInstance->CurrentRow.EliteEnemyDropMultiplier);
			}
			SpawnedEnemy->Initialize(SpawnLocation);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No Spawned Enemy"));
	}
}

void AEnemySpawnerTest::SpawnerInitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("EnemySpawnerTEST::SpawnerInitialize"));
	if (EnemyPool)
	{
		if (EnemyClass.IsValidIndex(MinionIndex) && EnemyClass[MinionIndex] != nullptr)
		{
			EnemyPool->InitializePool(PoolSize, EnemyClass[MinionIndex], MinionIndex);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("MinionIndex is null"));
		}

		if (EnemyClass.IsValidIndex(TankIndex) && EnemyClass[TankIndex] != nullptr)
		{
			EnemyPool->InitializePool(PoolSize, EnemyClass[TankIndex], TankIndex);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("TankIndex is null"));
		}

		if (EnemyClass.IsValidIndex(RangerIndex) && EnemyClass[RangerIndex] != nullptr)
		{
			EnemyPool->InitializePool(PoolSize, EnemyClass[RangerIndex], RangerIndex);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("RangerIndex is null"));
		}
	}
}

void AEnemySpawnerTest::SpawnerReset()
{
	UE_LOG(LogTemp, Warning, TEXT("EnemySpawnerTEST::SpawnerReset"));
	if (EnemyPool)
	{
		EnemyPool->ResetPool();
	}
}

void AEnemySpawnerTest::SpawnTimerStart()
{
	UE_LOG(LogTemp, Warning, TEXT("EnemySpawnerTEST::SpawnTimerStart"));
	PuckGameInstance->bIsHalf = false;
	GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &AEnemySpawnerTest::SpawnEnemy, 3, true);
}

void AEnemySpawnerTest::SpawnTimerClear()
{
	UE_LOG(LogTemp, Warning, TEXT("EnemySpawnerTEST::SpawnTimerClear"));
	PuckGameInstance->bIsHalf = true;
	GetWorld()->GetTimerManager().ClearTimer(SpawnHandle);
}

void AEnemySpawnerTest::SetWeightByLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("EnemySpawnerTEST::SetWeightByLevel"));
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
	UE_LOG(LogTemp, Warning, TEXT("EnemySpawnerTEST::SettingNewEnemy"));
	SetWeightByLevel();
	SpawnTimerClear();
	SpawnerReset();
	SpawnerInitialize();
	GetWorld()->GetTimerManager().SetTimer(HalfTimeHandle, this, &AEnemySpawnerTest::SpawnTimerStart, 10, false);
	//SpawnTimerStart();
}
