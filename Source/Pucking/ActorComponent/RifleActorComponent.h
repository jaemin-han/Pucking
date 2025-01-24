// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponent/GunActorComponent.h"
#include "RifleActorComponent.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PUCKING_API URifleActorComponent : public UGunActorComponent
{
	GENERATED_BODY()

public:
	URifleActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 초기화
	virtual void InitActorComponent() override;
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* RifleFireMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* RifleReloadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equip Parameter")
	FName RifleEquipSocket = "GunSocket";
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Equip Parameter")
	FTransform RifleEquipTransform = FTransform(FVector::ZeroVector);

public:
	// 부모의 Fire 메소드 구현
	UFUNCTION(BlueprintCallable)
	virtual void Fire(FVector StartLoc, FVector ForwardVector) override;

	// 부모의 Reload 메소드 구현
	UFUNCTION(BlueprintCallable)
	virtual void Reload() override;

	// 카메라 흔들림 메소드 구현
	UFUNCTION(BlueprintCallable)
	virtual void CameraShakeRecoil() override;
	
	// 조준 UI
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Crosshair UI")
	TSubclassOf<UCrosshairUI> CrosshairUIClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Crosshair UI")
	UCrosshairUI* CrosshairUI;

public:
	virtual TArray<struct FInputParameter> ReturnInputParameter() override;
	
	virtual void Input_Fire(const FInputActionValue& Value) override;

	virtual void Input_Reload() override;

	virtual void Start_ZoomIn() override;

	virtual void Start_ZoomOut() override;

private:
	// Tick에 따라 반동 변경
	UPROPERTY()
	float MultiplySpread = 1.f;

	// 사격 중인지 체크
	UPROPERTY()
	bool IsExtendSpread = false;

	// 집탄율 마이너스 보정값
	UPROPERTY()
	float FireExtendSpread = 0.f;

	// 집탄율 마이너스 보정값
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FireExtendPread", meta=(AllowPrivateAccess))
	float DefaultFireExtendSpread = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FireExtendPread", meta=(AllowPrivateAccess))
	float ZoomFireExtendSpread = 5.f;

	// UI와 사격값 보정치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="FireExtendPread", meta=(AllowPrivateAccess))
	float UIToFireLocation = 3.5f;

	// UI에 반영 될 캐릭터 속도 범위 
	TRange<float> InputSpreadRange;

	// UI에 반영 될 캐릭터 속도 범위 보정값 
	TRange<float> OutputSpreadRange;

	FTimerHandle SpreadTimerHandle;

public:
	virtual void DecreaseSpreadRange() override;
	virtual void IncreaseMaxMagazine() override;
	virtual void SetRateReloadAnimMontage() override;
	
};
