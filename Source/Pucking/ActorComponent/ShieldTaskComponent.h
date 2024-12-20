// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ShieldTaskComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUCKING_API UShieldTaskComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UShieldTaskComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	//class UStatusComponent* Status;
	float CurrentShield;

	FTimerHandle RecoveryDelayTimer;
	FTimerHandle RecoverySpeedTimer;
public:
	UFUNCTION(BlueprintCallable)
	void ShieldTakeDamage(float damage, EDamageType damageType);

	UFUNCTION(BlueprintCallable)
	void ShieldRecovery();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UStatusComponent* Status;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UHP_ManagementComponent* HP_Management;
};
