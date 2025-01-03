
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunActorComponent.h"
#include "ShotgunActorComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PUCKING_API UShotgunActorComponent : public UGunActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UShotgunActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ShotgunFireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ShotgunReloadMontage;

public:
	// 한번에 나가는 샷건 개수
	UPROPERTY(EditAnywhere)
	int32 BulletNum;

	// 부모의 Fire 메소드 구현
	UFUNCTION(BlueprintCallable)
	virtual void Fire(FVector StartLoc, FVector ForwardVector) override;

	// 부모의 Reload 메소드 구현
	UFUNCTION(BlueprintCallable)
	virtual void Reload() override;

	UFUNCTION(BlueprintCallable)
	virtual void SetSpreadRange(float Y, float Z) override;

	// 카메라 흔들림 메소드 구현
	UFUNCTION(BlueprintCallable)
	virtual void CameraShakeRecoil() override;

public:
	virtual TArray<struct FInputParameter> ReturnInputParameter() override;
	
	virtual void Input_Fire(const FInputActionValue& Value) override;

	virtual void Input_Reload() override;
};
