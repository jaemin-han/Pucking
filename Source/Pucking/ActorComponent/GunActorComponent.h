// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/EquipInterface.h"
#include "Interfaces/FireInterface.h"
#include "Interfaces/ReloadInterface.h"
#include "Common/CommonStruct.h"
#include "Common/CommonEnum.h"
#include "Interfaces/BindInputInterface.h"
#include "GunActorComponent.generated.h"

DECLARE_DELEGATE_RetVal_OneParam(bool, FOnIsRemainAmmo, int32);
DECLARE_DELEGATE_RetVal_OneParam(int32, FOnRemainAmmo, int32);

class UStaticMeshComponent;
class UInputAction;
class UAnimMontage;
class UCrosshairUI;

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
	
	// 초기화
	virtual void InitActorComponent();

public: 
	// EquipComponent에서 장전 가능한지 여부를 반환받는 Delegate
	FOnIsRemainAmmo OnIsRemainAmmo;

	// EquipComponent에서 남은 총알 수를 반환받는 Delegate
	FOnRemainAmmo OnRemainAmmo;
	
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

	// 설정한 SkeletalMesh로 Owner에게 Attach 해줄 SkeletalMeshComponent
	UPROPERTY(VisibleAnywhere, Category = "GunActorCompo SkeletalMeshComponent")
	USkeletalMeshComponent* SkeletalMeshComponent;

	// 총 포구 Particle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Muzzle Effect")
	UParticleSystem* MuzzleParticle;

	// 총 공격 Particle
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire Effect")
	UParticleSystem* FireParticle;

protected:
	// 사격 가능 상태 여부
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Is ShootAble")
	bool bIsShootAble;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Is Aiming")
	bool bIsAiming = false;
	
	// 총 Input 관련 FInputParameter 구조체
	UPROPERTY()
	TArray<FInputParameter> InputParameters;

	// Owner에게 추가해줄 InputMappingContext
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun InputMappingContext")
	class UInputMappingContext* GunInputMappingContext;

	// 사격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun InputAction")
	UInputAction* FireInputAction;

	// 장전
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun InputAction")
	UInputAction* ReloadInputAction;

	// 줌
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun InputAction")
	UInputAction* ZoomAction;

	// 몽타주 재생할 때 필요한 Owner 정보
	UPROPERTY()
	class ACharacter* OwnerCharacter;

	// ActorComponent의 타입
	UPROPERTY()
	EWeaponType WeaponType;

	// 현재 Owner의 무기 타입
	UPROPERTY()
	EWeaponType PlayerWeaponType;

	// Visibility 조절하는 부모 UI 변수
	UPROPERTY()
	UUserWidget* CrosshairWidget;
	
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
	bool GetIsShootAble();
	
	// 사격 가능 상태 조절
	void SetIsShootAble(bool ShootAble);

	// Camera Shake
	virtual void CameraShakeRecoil();

	// Player의 무기가 바꼈을 때 Delegate
	UFUNCTION()
	virtual void SetCurrentOwnerWeaponType(EWeaponType ChangeWeaponType);

	// Player의 Aiming 상태
	UFUNCTION()
	void SetIsAiming(bool CurrentAiming);

	UFUNCTION()
	bool GetIsAiming() const;

	// 몽타주 실행
	void PlayOwnerMontage(class UAnimMontage* OwnerMontage, float InRate);

protected:
	// 줌 상태에 따른 Default 사격 반동 보정값(Y) 반환
	float GetSpreadYRange();
	
	// 줌 상태에 따른 Default 사격 반동 보정값(Z) 반환
	float GetSpreadZRange();

	// 장전 애님 몽타주 배속 비율
	float RateReloadMontage = 1.0f;

public:
	UFUNCTION()
	virtual void Input_Fire(const FInputActionValue& Value);

	UFUNCTION()
	virtual void Input_Reload();

	UFUNCTION()
	virtual void Start_ZoomIn();

	UFUNCTION()
	virtual void Start_ZoomOut();

	// Input에 필요한 Struct 배열 Return
	UFUNCTION()
	virtual TArray<FInputParameter> ReturnInputParameter() override;

public:
	// 강화 옵션
	// 집탄 범위 조절
	UFUNCTION(BlueprintCallable)
	virtual void IncreaseSpreadRange(float Y, float Z);

	// 탄창 개수 증가
	UFUNCTION(BlueprintCallable)
	virtual void IncreaseMaxMagazine(int32 ChangeMagazine);

	// 연사 속도 증가
	UFUNCTION(BlueprintCallable)
	virtual void IncreaseShootInterval(float ChangeShootInterval);

	// 샷건 총 개수 증가
	UFUNCTION(BlueprintCallable)
	virtual void IncreaseShotgunBulletNum(int32 ShotgunBullet);

	// 장전 애님몽타주 배속 설정
	UFUNCTION(BlueprintCallable)
	virtual void SetRateReloadAnimMontage(float InRate);
};
