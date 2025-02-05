// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyProjectileBase.h"

#include "ActorComponent/StatusComponent.h"
#include "Components/BoxComponent.h"
#include "Interfaces/StatusInterface.h"

void AEnemyProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	BoxComp = FindComponentByClass<UBoxComponent>();
	BoxComp->OnComponentHit.AddDynamic(this, &AEnemyProjectileBase::OnBoxComponentHit);
}

void AEnemyProjectileBase::OnBoxComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	bool bIsHit = Hit.bBlockingHit;
	if(bIsHit)
	{
		if(AActor* HitActor = Hit.GetActor())
		{
			IStatusInterface* StatInterface = Cast<IStatusInterface>(GetInstigator()->FindComponentByClass<UStatusComponent>());
			if(StatInterface)
			{
				StatInterface->DamageProcessing(HitActor, Hit);
			}
		}
	}
	Destroy();
}