// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Weapon.h"

#include "ActorComponent/CloseCombatComponent.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	//MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	CloseCombatComp = CreateDefaultSubobject<UCloseCombatComponent>(TEXT("CloseCombatComp"));
	RootComponent = CloseCombatComp;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}