// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableItem.h"

#include "Components/SphereComponent.h"
#include "World/PuckGameState.h"


class APuckGameState;
// Sets default values
APickableItem::APickableItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LightBeam = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightBeam"));

	// SphereCollision 과 GameTraceChannel1 을 을 block
	SphereCollision->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Block);

	// lifetime
	InitialLifeSpan = 60.0f;
}

// Called when the game starts or when spawned
void APickableItem::BeginPlay()
{
	Super::BeginPlay();
}

void APickableItem::ConstructMesh() const
{
	Super::ConstructMesh();

	// GameState 가져오기
	APuckGameState* PuckGameState = GetWorld()->GetGameState<APuckGameState>();

	// ItemData 의 ItemRarity 에 따라 Material 을 설정
	if (PuckGameState)
	{
		switch (ItemData.ItemRarity)
		{
		case EItemRarity::Normal:
			LightBeam->SetMaterial(0, PuckGameState->MaterialArray[0]);
			break;
		case EItemRarity::Magic:
			LightBeam->SetMaterial(0, PuckGameState->MaterialArray[1]);
			break;
		case EItemRarity::Rare:
			LightBeam->SetMaterial(0, PuckGameState->MaterialArray[2]);
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Invalid ItemRarity"));
			break;
		}
	}
}

// Called every frame
void APickableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
