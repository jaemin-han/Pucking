// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/StatusComponent.h"
#include "Interfaces/StatusInterface.h"
#include "EnemyStatusComponent.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API UEnemyStatusComponent : public UStatusComponent, public IStatusInterface
{
	GENERATED_BODY()
	
public:
	virtual void DamageProcessing(AActor* hitActor) override;
};
