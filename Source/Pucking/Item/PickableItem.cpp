// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableItem.h"

#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
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

	// LightBeam 는 충돌하지 않는다
	LightBeam->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void APickableItem::BeginPlay()
{
	Super::BeginPlay();
}

void APickableItem::ConstructMesh()
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
			LightBeam->SetMaterial(0, PuckGameState->PickableMaterials[0]);
			break;
		case EItemRarity::Magic:
			LightBeam->SetMaterial(0, PuckGameState->PickableMaterials[1]);
			break;
		case EItemRarity::Rare:
			LightBeam->SetMaterial(0, PuckGameState->PickableMaterials[2]);
			break;
		default:
			//UE_LOG(LogTemp, Warning, TEXT("Invalid ItemRarity"));
			break;
		}
	}
}

void APickableItem::SetItemData(const struct FItemDropData& ItemDropData)
{
	Super::SetItemData(ItemDropData);


	switch (ItemType)
	{
	case EItemType::Ammo:
		{
			PickableData = MakeShared<FAmmoData>();
			FAmmoData* AmmoData = static_cast<FAmmoData*>(PickableData.Get());
			AmmoData->AmmoCount = ItemDropData.AmmoData.AmmoCount;
			AmmoData->DamageType = ItemDropData.AmmoData.DamageType;
			AmmoData->WeaponType = ItemDropData.AmmoData.WeaponType;
			AmmoData->AmmoDamage = ItemDropData.AmmoData.AmmoDamage;
			AmmoData->CriticalRate = ItemDropData.AmmoData.CriticalRate;
			AmmoData->CriticalMultiplier = ItemDropData.AmmoData.CriticalMultiplier;
			AmmoData->StaggerValue = ItemDropData.AmmoData.StaggerValue;
			break;
		}
	default:
		{
			UE_LOG(LogTemp, Error, TEXT("Invalid ItemType"));
			break;
		}
	}
}

// Called every frame
void APickableItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APickableItem::PostInitialize()
{
	Super::PostInitialize();

	APuckGameState* PuckGameState = GetWorld()->GetGameState<APuckGameState>();

	// PickableData 가 AmmoData 이고, 해당 Ammo 가 BFG 일 경우, LightBeam 의 Material 을 설정
	if (PickableData.IsValid())
	{
		// GameState 가져오기

		FAmmoData* AmmoData = static_cast<FAmmoData*>(PickableData.Get());
		if (AmmoData->WeaponType == EWeaponType::BFG && AmmoData)
		{
			LightBeam->SetMaterial(0, PuckGameState->PickableMaterials[3]);

			// LightBeam 의 scale 을 xy 는 10배, z 는 2배로 설정
			LightBeam->SetWorldScale3D(FVector(10.f, 10.f, 2.f));
		}
	}
}

void APickableItem::AfterStop()
{
	APuckGameState* PuckGameState = GetWorld()->GetGameState<APuckGameState>();


	// 아이템 희귀도가 Magic 이면 PickableSounds[0]
	// 아이템 희귀도가 Rare 이면 PickableSounds[1]
	// BFG 일 경우 PickableSounds[2]
	// 사운드는 2D 사운드로 재생
	if (PuckGameState)
	{
		switch (ItemData.ItemRarity)
		{
		case EItemRarity::Normal:
			UGameplayStatics::PlaySound2D(GetWorld(), PuckGameState->PickableSounds[0],
			                              PuckGameState->SoundVolumeMultiplier);
			break;
		case EItemRarity::Magic:
			UGameplayStatics::PlaySound2D(GetWorld(), PuckGameState->PickableSounds[1],
			                              PuckGameState->SoundVolumeMultiplier);
			break;
		case EItemRarity::Rare:
			UGameplayStatics::PlaySound2D(GetWorld(), PuckGameState->PickableSounds[2],
			                              PuckGameState->SoundVolumeMultiplier);
			break;
		default:
			//UE_LOG(LogTemp, Warning, TEXT("Invalid ItemRarity"));
			break;
		}
	}

	// PickableData 가 AmmoData 이고, 해당 Ammo 가 BFG 일 경우, PickableSounds[2] 재생
	if (PickableData.IsValid())
	{
		FAmmoData* AmmoData = static_cast<FAmmoData*>(PickableData.Get());
		if (AmmoData->WeaponType == EWeaponType::BFG && AmmoData)
		{
			UGameplayStatics::PlaySound2D(GetWorld(), PuckGameState->PickableSounds[3],
			                              PuckGameState->SoundVolumeMultiplier);
		}
	}
}
