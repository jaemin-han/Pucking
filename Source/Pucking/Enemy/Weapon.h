// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class PUCKING_API AWeapon : public AActor
{
	GENERATED_BODY()
	//////////////Function/////////////////
public:	
	AWeapon();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	//////////////Variable/////////////////
public:
protected:
private:
};
