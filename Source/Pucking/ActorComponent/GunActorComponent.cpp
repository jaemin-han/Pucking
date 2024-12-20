// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/GunActorComponent.h"

// Sets default values for this component's properties
UGunActorComponent::UGunActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGunActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGunActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UGunActorComponent::Equip(USkeletalMeshComponent* TargetSkeletalMeshComp, FName SocketName,
	FTransform ActorTransform)
{
}

void UGunActorComponent::Fire(class UArrowComponent* GunArrowComponent)
{
}

void UGunActorComponent::Reload(class FShotgunInfo& GunInfo)
{
}
