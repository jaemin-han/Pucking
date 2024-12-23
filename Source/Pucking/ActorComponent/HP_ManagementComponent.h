// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HP_ManagementComponent.generated.h"




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUCKING_API UHP_ManagementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHP_ManagementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	//class UStatusComponent* Status;

	float CurrentHP;

public:
	UFUNCTION(BlueprintCallable)
	void HPTakeDamage(float damage, EDamageType damageType);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStatusComponent* Status;
};
