// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/CloseCombatComponent.h"

UCloseCombatComponent::UCloseCombatComponent()
{
	CloseCombatMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CloseCombatMeshComp"));
	//HitBox->SetupAttachment(this);
}

void UCloseCombatComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerActor = GetOwner();
	if(OwnerActor)
	//HitBox = NewObject<UBoxComponent>(OwnerActor);
	{
		//HitBox->RegisterComponent();
		//HitBox->SetupAttachment(OwnerActor->GetRootComponent());
		FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
		CloseCombatMeshComp->AttachToComponent(OwnerActor->GetRootComponent(),TransformRules);
	}
}