// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AOE/ProjectileBase.h"
#include "EnemyProjectileBase.generated.h"

/**
 * 
 */

UCLASS()
class PUCKING_API AEnemyProjectileBase : public AProjectileBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UBoxComponent* BoxComp;
	
	UFUNCTION()
	void OnBoxComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );
};
