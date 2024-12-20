// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GunActorComponent.h"
#include "ShotGunActorComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PUCKING_API UShotGunActorComponent : public UGunActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UShotGunActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 부모의 Equip 메소드 구현
	UFUNCTION(BlueprintCallable)
	virtual void Equip(USkeletalMeshComponent* TargetSkeletalMeshComp, FName SocketName, FTransform ActorTransform) override;

	// 부모의 Fire 메소드 구현
	virtual void Fire(class UArrowComponent* GunArrowComponent) override;

	// 부모의 Reload 메소드 구현
	virtual void Reload(class FShotgunInfo& GunInfo) override;
};
