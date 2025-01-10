// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableItem.h"

#include "Components/SphereComponent.h"


// Sets default values
APickableItem::APickableItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// SphereCollision 과 GameTraceChannel1 을 을 block
	SphereCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);
}

// Called when the game starts or when spawned
void APickableItem::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void APickableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

