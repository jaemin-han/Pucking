// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "CloseCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API UCloseCombatComponent : public UBoxComponent
{
	GENERATED_BODY()
public:	
	UCloseCombatComponent();
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	AActor* OwnerActor;
	
	UPROPERTY(EditAnywhere, Category= "WeaponProperties")
	UStaticMeshComponent* CloseCombatMeshComp;
};
