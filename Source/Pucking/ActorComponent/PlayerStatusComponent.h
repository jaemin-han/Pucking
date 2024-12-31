// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/StatusComponent.h"
#include "Interfaces/StatusInterface.h"
#include "PlayerStatusComponent.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API UPlayerStatusComponent : public UStatusComponent, public IStatusInterface
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	float DamageAmount;
public:
	virtual void DamageProcessing(AActor* hitActor) override;
	virtual void GetDamage(float damageAmount, float Penetration) override;
};
