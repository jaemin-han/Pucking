// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/GunActorComponent.h"
#include "BFGActorComponent.generated.h"

/**
 * 
 */
class UShotgunUI;
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PUCKING_API UBFGActorComponent : public UGunActorComponent
{
	GENERATED_BODY()
public:
	UBFGActorComponent();

	virtual void InitActorComponent() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void Fire(FVector StartLoc, FVector ForwardVector) override;

	UFUNCTION(BlueprintCallable)
	virtual void Reload();

	virtual TArray<struct FInputParameter> ReturnInputParameter() override;
	
	virtual void Input_Fire(const FInputActionValue& Value) override;
	
	virtual void Input_Reload() override;

	UFUNCTION(BlueprintCallable)
	void LevelUp();

	// 카메라 흔들림
	UFUNCTION(BlueprintCallable)
	virtual void CameraShakeRecoil() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Level_BFG = 0;

	// 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* BFGFireMontage;

	// Projectile
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AProjectileBase> BFGProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AProjectileBase>> BFGProjectiles_Level;
};