// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/EquipInterface.h"
#include "Interfaces/FireInterface.h"
#include "Interfaces/ReloadInterface.h"
#include "Common/CommonStruct.h"
#include "Interfaces/BindInputInterface.h"
#include "GunActorComponent.generated.h"

DECLARE_DELEGATE_RetVal_OneParam(int32, FOnRemainAmmo, int32);
DECLARE_DELEGATE(FOnInputFire);
DECLARE_DELEGATE(FOnInputReload);

class UStaticMeshComponent;
class UInputAction;
class UAnimMontage;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUCKING_API UGunActorComponent : public UActorComponent, public IFireInterface, public IReloadInterface, public IEquipInterface, public IBindInputInterface
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

	// EquipComponent에서 남은 총알 수를 반환받는 Delegate
	FOnRemainAmmo OnRemainAmmo;

	// Fire Input 입력 시
	FOnInputFire OnInputFire;

	// Reload Input 입력 시
	FOnInputReload OnInputReload;
	
public:
	// 총 기본 데이터 테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GunActorCompo DataTable")
	UDataTable* GunInfoDataTable;

	// 총 정보 구조체
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category="GunActorCompo Struct")
	FGunInfoStruct GunInfoStruct;

	// 총의 기본 SkeletalMesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="GunActorCompo SkeletalMesh")
	USkeletalMesh* GunSkeletalMesh;

	// 총 포구 Particle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Muzzle Effect")
	UParticleSystem* MuzzleParticle;

	// 총 공격 Particle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Effect")
	UParticleSystem* FireParticle;

	// Owner의 Camera Component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gun Owner Camera")
	class UCameraComponent* OwnerCameraComp;

	// 사격 가능 상태
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Is ShootAble")
	bool bIsShootAble;

protected:
	// 총 Input 관련 Parameter
	// FInputParameter 구조체
	UPROPERTY()
	TArray<FInputParameter> InputParameters;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun InputMappingContext")
	class UInputMappingContext* GunInputMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun InputAction")
	UInputAction* FireInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun InputAction")
	UInputAction* ReloadInputAction;

	UPROPERTY()
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY()
	class ACharacter* OwnerCharacter;
	
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

public:
	UFUNCTION()
	virtual void Input_Fire(const FInputActionValue& Value);

	UFUNCTION()
	virtual TArray<FInputParameter> ReturnInputParameter() override;
	
};
