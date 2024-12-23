// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotGunActorComponent.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

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
	
	if(GunStaticMesh)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

		if (UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(TargetSkeletalMeshComp->GetOwner()))
		{
			// StaticMesh 정보를 StaticMeshComponent에 
			StaticMeshComponent->SetStaticMesh(GunStaticMesh);

			// SkeletalMesh에 붙이기
			StaticMeshComponent->AttachToComponent(TargetSkeletalMeshComp, AttachmentRules, SocketName);

			// Component 등록하기
			StaticMeshComponent->RegisterComponent();
		}
	}
}

void UShotGunActorComponent::Fire(FVector StartLoc, FVector FrontVelocity)
{
	Super::Fire(StartLoc, FrontVelocity);
	
	FVector EndLoc = StartLoc + FrontVelocity * ShotgunInfo.Range;

	for(int i=0; i < BulletNum; i++)
	{
		FHitResult _hitRes;

		FCollisionQueryParams _collisionParam;
		_collisionParam.AddIgnoredActor(GetOwner());

		EndLoc.X += FMath::RandRange(ShotgunInfo.RecoilX * -1, ShotgunInfo.RecoilX);
		EndLoc.Y += FMath::RandRange(ShotgunInfo.RecoilY * -1, ShotgunInfo.RecoilY);
		EndLoc.Z += FMath::RandRange(ShotgunInfo.RecoilZ * -1, ShotgunInfo.RecoilZ);
		
		bool isHit = GetWorld()->LineTraceSingleByChannel(_hitRes, StartLoc, EndLoc, ECC_Pawn, _collisionParam);
		DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Green, true, 5.f);
		
		if(isHit)
		{
			if(AActor* hitActor = _hitRes.GetActor())
			{
				UGameplayStatics::ApplyDamage(hitActor, ShotgunInfo.DefaultDamage, nullptr, nullptr, UDamageType::StaticClass());
			}
		}
	}
}

void UShotGunActorComponent::Reload(FShotgunInfo& GunInfo)
{
	Super::Reload(GunInfo);
}

