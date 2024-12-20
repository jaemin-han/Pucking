// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotGunActorComponent.h"

#include "GameFramework/Character.h"

// Sets default values for this component's properties
UShotGunActorComponent::UShotGunActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UShotGunActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UShotGunActorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UShotGunActorComponent::Equip(USkeletalMeshComponent* TargetSkeletalMeshComp, FName SocketName, FTransform ActorTransform)
{
	Super::Equip(TargetSkeletalMeshComp, SocketName, ActorTransform);
	UE_LOG(LogTemp, Warning, TEXT("Equip Start"));
	if(this->GunStaticMesh)
	{
		UE_LOG(LogTemp, Warning, TEXT("Has Mesh"));
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		//this->GunStaticMeshComponent->AttachToComponent(TargetSkeletalMeshComp, AttachmentRules, SocketName);

		if (UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(TargetSkeletalMeshComp->GetOwner()))
		{
			StaticMeshComponent->SetStaticMesh(GunStaticMesh);

			// Attach the StaticMeshComponent to the SkeletalMeshComponent's socket
			StaticMeshComponent->AttachToComponent(TargetSkeletalMeshComp, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);

			// Register the new component so it appears in the world
			StaticMeshComponent->RegisterComponent();

			UE_LOG(LogTemp, Log, TEXT("Successfully attached StaticMesh to socket %s on SkeletalMeshComponent %s"), *SocketName.ToString(), *TargetSkeletalMeshComp->GetName());
		}
	}
}

void UShotGunActorComponent::Fire(class UArrowComponent* GunArrowComponent)
{
	Super::Fire(GunArrowComponent);
}

void UShotGunActorComponent::Reload(class FShotgunInfo& GunInfo)
{
	Super::Reload(GunInfo);
}

