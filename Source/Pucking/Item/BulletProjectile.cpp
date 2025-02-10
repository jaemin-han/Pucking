// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/BulletProjectile.h"

#include "Components/BoxComponent.h"

void ABulletProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	BoxComp = FindComponentByClass<UBoxComponent>();
	BoxComp->OnComponentHit.AddDynamic(this, &ABulletProjectile::OnBoxComponentHit);
}

void ABulletProjectile::OnBoxComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}