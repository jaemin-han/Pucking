// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/EquipInterface.h"
#include "Interfaces/FireInterface.h"
#include "Interfaces/ReloadInterface.h"
#include "Common/CommonStruct.h"
#include "GunActorComponent.generated.h"

class UStaticMeshComponent;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUCKING_API UGunActorComponent : public UActorComponent, public IFireInterface, public IReloadInterface, public IEquipInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGunActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 총 기본 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GunActorCompo DataTable")
	UDataTable* GunInfoDataTable;

	// 총 정보 구조체
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="GunActorCompo Struct")
	FGunInfoStruct GunInfoStruct;

	// 총의 기본 StaticMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GunActorCompo StaticMesh")
	UStaticMesh* GunStaticMesh;

	// 총 포구 Particle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Muzzle Effect")
	UParticleSystem* MuzzleParticle;

	// 총 공격 Particle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Effect")
	UParticleSystem* FireParticle;

	// 사격 가능 상태
	UPROPERTY()
	bool bIsShootAble;

	// Actor에 부착한 StaticMesh Component
	UPROPERTY()
	UStaticMeshComponent* GunStaticMeshComponent;

public:
	// 총 기본 정보를 담고 있는 Struct 정보를 세팅
	virtual void SetDefaultGunInfoStruct(FName TableRows);
	
	// 장착할 Actor와 SocketName, Transform 정보
	virtual void Equip(USkeletalMeshComponent* TargetSkeletalMeshComp, FName SocketName, FTransform ActorTransform) override;
	
	// Gun StaticMesh에 격발
	virtual void Fire(FVector StartLoc, FVector ForwardVector) override;

	// Gun Struct의 탄환 값을 늘려준다
	virtual void Reload() override;

	// 총 발사 간격 조절
	void SetShootInterval(float IntervalTime);
	
	// 사격 가능 상태
	bool GetIsShootAble() const;
	
	// 사격 가능 상태 조절
	void SetIsShootAble(bool ShootAble);

	// 집탄 범위 조절
	virtual void SetSpreadRange(float Y, float Z);

	// Camera Shake
	virtual void CameraShakeRecoil();
	
	// EquipActorComponent에서 Delegate Broadcast하면 호출
	UFUNCTION()
	void BindChangeAmmoEvent();
};
