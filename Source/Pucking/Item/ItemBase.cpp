// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

#include "Components/SphereComponent.h"


// Sets default values
AItemBase::AItemBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Item Static Mesh
	ItemStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemStaticMesh"));
	SetRootComponent(ItemStaticMesh);

	// Item Skeletal Mesh
	ItemSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemSkeletalMesh"));
	ItemSkeletalMesh->SetupAttachment(RootComponent);

	// Sphere Collision
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(ItemSkeletalMesh);

	// set gravity
	ItemStaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemStaticMesh->SetSimulatePhysics(true);
	ItemStaticMesh->SetEnableGravity(true);
	ItemStaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	ItemSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemSkeletalMesh->SetSimulatePhysics(true);
	ItemSkeletalMesh->SetEnableGravity(true);
	ItemSkeletalMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	// Static Mesh 와 Visibility의 Trace Response 를 Ignore 로 설정, OverlapItem 의 line trace 와 충돌하지 않도록 설정
	ItemStaticMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);

	// ItemStaticMesh 의 Collision Preset 을 ItemPreset 으로 설정
	ItemStaticMesh->SetCollisionProfileName(TEXT("ItemPreset"));
	// ItemSkeletalMesh 의 Collision Preset 을 ItemPreset 으로 설정
	ItemSkeletalMesh->SetCollisionProfileName(TEXT("ItemPreset"));
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
}

void AItemBase::OnPickup()
{
	// 자식 클래스에서 구현
}

void AItemBase::ApplySpeed()
{
	if (ItemStaticMesh && ItemStaticMesh->IsSimulatingPhysics())
	{
		// z축 속도 설정 (예: 위로 밀어 올리는 속도)
		float ZVelocity = 300.0f; // 원하는 값으로 조절하세요

		// xy 평면에서의 속도 크기 설정
		float XYVelocityMagnitude = 300.0f; // 원하는 값으로 조절하세요

		// 0도부터 360도 사이의 랜덤 각도 생성
		float AngleDegrees = FMath::RandRange(0.0f, 360.0f);
		float AngleRadians = FMath::DegreesToRadians(AngleDegrees);

		// 랜덤 각도를 이용해 xy축 속도 분해
		float XVelocity = XYVelocityMagnitude * FMath::Cos(AngleRadians);
		float YVelocity = XYVelocityMagnitude * FMath::Sin(AngleRadians);

		// 최종 속도 벡터 생성
		FVector InitialVelocity(XVelocity, YVelocity, ZVelocity);

		// 물체의 속도 설정
		ItemStaticMesh->SetPhysicsLinearVelocity(InitialVelocity);
	}
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemBase::ConstructMesh()
{
	// item data 의 ItemStaticMesh 를 ItemStaticMesh 에 적용
	ItemStaticMesh->SetStaticMesh(ItemData.ItemStaticMesh);

	// item data 의 ItemSkeletalMesh 를 ItemSkeletalMesh 에 적용
	ItemSkeletalMesh->SetSkeletalMesh(ItemData.ItemSkeletalMesh);

	ApplySpeed();
}

void AItemBase::SetItemData(const FItemDropData& ItemDropData)
{
	ItemData.ItemName = ItemDropData.ItemName;
	ItemData.ItemStaticMesh = ItemDropData.ItemStaticMesh;
	ItemData.ItemSkeletalMesh = ItemDropData.ItemSkeletalMesh;
	ItemData.ItemType = ItemDropData.ItemType;
	ItemData.ItemThumbnail = ItemDropData.ItemThumbnail;
	ItemData.bStackable = ItemDropData.bStackable;
	ItemData.MaxStackCount = ItemDropData.MaxStackCount;

	ItemType = ItemDropData.ItemType;

	ConstructMesh();
}
