// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"

#include "Chaos/Deformable/ChaosDeformableCollisionsProxy.h"
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
	
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItemBase::ConstructMesh() const
{
	// item data 의 ItemStaticMesh 를 ItemStaticMesh 에 적용
	ItemStaticMesh->SetStaticMesh(ItemData.ItemStaticMesh);

	// item data 의 ItemSkeletalMesh 를 ItemSkeletalMesh 에 적용
	ItemSkeletalMesh->SetSkeletalMesh(ItemData.ItemSkeletalMesh);
}
