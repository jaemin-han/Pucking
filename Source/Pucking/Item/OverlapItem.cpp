// Fill out your copyright notice in the Description page of Project Settings.


#include "OverlapItem.h"

#include "Components/SphereComponent.h"
#include "Interfaces/EssenceInterface.h"
#include "Interfaces/HealthMarbleInterface.h"


// Sets default values
AOverlapItem::AOverlapItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AOverlapItem::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(CheckCollisionTimerHandle, this, &AOverlapItem::CheckCollision, 0.1f, true);
	// Lambda를 사용하여 3초 후에 CheckCollisionTimerHandle을 중지
	// 혹시라도 아이템 위에 아이템이 올라가 있어 타이머가 중지되지 않는 경우를 방지
	FTimerHandle StopTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(StopTimerHandle, FTimerDelegate::CreateLambda([this]()
	{
		// 물리 시뮬레이션 및 중력 비활성화
		if (IsValid(ItemStaticMesh))
		{
			ItemStaticMesh->SetSimulatePhysics(false);
			ItemStaticMesh->SetEnableGravity(false);
		}

		// Timer 정지
		GetWorld()->GetTimerManager().ClearTimer(CheckCollisionTimerHandle);
	}), 3.0f, false);
}

void AOverlapItem::OnPickup()
{
	Super::OnPickup();
	if (ItemType == EItemType::Essence)
	{
	}
	else if (ItemType == EItemType::HealthMarble)
	{
	}
	Destroy();
}

// Called every frame
void AOverlapItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOverlapItem::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                  UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                  const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("AOverlapItem: Overlap Begin, OtherActor: %s"), *OtherActor->GetName());

	// ItemType 이 HealthMarble 일 경우, 체력 회복
	if (ItemType == EItemType::HealthMarble)
	{
		IHealthMarbleInterface* HealthMarbleInterface = Cast<IHealthMarbleInterface>(OtherActor);
		if (!HealthMarbleInterface)
			return;
		float HealthRecovery = static_cast<FHealthMarbleData*>(OverlapData.Get())->HealthRecovery;
		HealthMarbleInterface->ApplyHeal(HealthRecovery);
		OnPickup();
	}
	else if (ItemType == EItemType::Essence)
	{
		// EssenceInterface를 상속받은 Actor인지 확인
		IEssenceInterface* EssenceInterface = Cast<IEssenceInterface>(OtherActor);
		if (!EssenceInterface)
			return;
		int32 Essence = static_cast<FEssenceData*>(OverlapData.Get())->EssenceCount;
		EssenceInterface->AddEssence(Essence);
		OnPickup();
	}
}

void AOverlapItem::CheckCollision()
{
	// UE_LOG(LogTemp, Warning, TEXT("AOverlapItem: Checking Collision"));

	// Actor 위치에서 아래로 150만큼 LineTrace 수행
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.f, 0.f, 100.f);
	FHitResult HitResult;
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility,
	                                     CollisionQueryParams);

	// 충돌 감지 시 처리
	if (HitResult.GetActor())
	{
		// UE_LOG(LogTemp, Warning, TEXT("AOverlapItem: HitResult Actor: %s"), *HitResult.GetActor()->GetName());

		// 물리 시뮬레이션 및 중력 비활성화
		ItemStaticMesh->SetSimulatePhysics(false);
		ItemStaticMesh->SetEnableGravity(false);

		// Timer 정지
		GetWorld()->GetTimerManager().ClearTimer(CheckCollisionTimerHandle);
	}
}

void AOverlapItem::OnInitialize()
{
	SphereCollision->SetSphereRadius(OverlapData.Get()->OverlapRadius);
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AOverlapItem::OnOverlapBegin);
}

void AOverlapItem::SetItemData(const struct FItemDropData& ItemDropData)
{
	Super::SetItemData(ItemDropData);


	switch (ItemType)
	{
	case EItemType::Essence:
		{
			OverlapData = MakeShared<FEssenceData>();
			FEssenceData* EssenceData = static_cast<FEssenceData*>(OverlapData.Get());
			EssenceData->EssenceCount = ItemDropData.EssenceData.EssenceCount;
			EssenceData->LifeTime = ItemDropData.EssenceData.LifeTime;
			EssenceData->OverlapRadius = ItemDropData.EssenceData.OverlapRadius;
			break;
		}
	case EItemType::HealthMarble:
		{
			OverlapData = MakeShared<FHealthMarbleData>();
			FHealthMarbleData* HealthMarbleData = static_cast<FHealthMarbleData*>(OverlapData.Get());
			HealthMarbleData->HealthRecovery = ItemDropData.HealthMarbleData.HealthRecovery;
			HealthMarbleData->LifeTime = ItemDropData.HealthMarbleData.LifeTime;
			HealthMarbleData->OverlapRadius = ItemDropData.HealthMarbleData.OverlapRadius;
			break;
		}
	default:
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid ItemType"));
			break;
		}
	}
	OnInitialize();
}
