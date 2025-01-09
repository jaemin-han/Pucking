// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Weapon.h"

#include "ActorComponent/CloseCombatComponent.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
	
	CloseCombatComp = CreateDefaultSubobject<UCloseCombatComponent>(TEXT("CloseCombatComp"));
	CloseCombatComp->SetupAttachment(RootComponent);
	CloseCombatComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//CloseCombatComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::OnCombatMeshAttachment(UStaticMeshComponent* TargetMeshComp, USceneComponent* BoxTraceStart, USceneComponent* BoxTraceEnd)
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::KeepRelative, true);
	if(CloseCombatComp && TargetMeshComp && BoxTraceStart && BoxTraceEnd)
	{
		TargetMeshComp->AttachToComponent(CloseCombatComp, TransformRules);
		BoxTraceStart->AttachToComponent(CloseCombatComp, TransformRules);
		BoxTraceEnd->AttachToComponent(CloseCombatComp, TransformRules);
	}
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}