// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "Components/ActorComponent.h"
#include "Interfaces/BindInputInterface.h"
#include "Interfaces/EquipInterface.h"
#include "JetPackMovementComponent.generated.h"


class UInputAction;

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUCKING_API UJetPackMovementComponent : public UActorComponent, public IBindInputInterface, public IEquipInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UJetPackMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="JetPack Actor")
	TSubclassOf<AActor> JetPackActorComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="JetPack Actor")
	UChildActorComponent* JetPackActor;

	// 비행 중
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsFlying = false;;

	// 비행 쿨타임 상태
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsFlyingCool = false;

	// 쿨타임
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float FlyingCoolTime = 5.f;

	// 전체 에너지
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="JetPack Energy")
	float TotalEnergy = 100.f;

	// 에너지 증감량
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="JetPack Energy")
	float ChangeEnergy = 0.3f;
	
	// 현재 에너지
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category="JetPack Energy")
	float CurEnergy;

	// 앞뒤 이동
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Flying Movement")
	float InputForwardValue;

	// 좌우 이동
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Flying Movement")
	float InputRightValue;

	// 상하 이동
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Flying Movement")
	float InputUpValue;

	// 회전
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Flying Movement")
	float InputTurnValue;

	// 비행 중일 때 캐릭터 회전값
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Flying Movement")
	float FlyingRotationRateZ = 260.f;

	// 평소 캐릭터 회전값
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Flying Movement")
	float WalkingRotationRateZ = 540.f;

	// 상승 값
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Flying Movement")
	float UpFlyingForce = 2.f;

	// 하강 값
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="Flying Movement")
	float DownFlyingForce = 2.f;
	
	// Owner 정보
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Owner Information")
	class ACharacter* OwnerCharacter;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Owner Information")
	class UAnimInstance* CharAnimInstance;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="JetPack Input")
	class UInputMappingContext* JetPackInputContext;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="JetPack Input")
	UInputAction* FlyingInputAction;

	// JetpackUI
	UPROPERTY()
	class USubCoolTimeUI* SubCoolTimeUI;
	
public:
	// Mesh Equip
	UFUNCTION()
	virtual void Equip(USkeletalMeshComponent* TargetSkeletalMeshComp, FName SocketName, FTransform ActorTransform) override;

	// Input Bind Param
	UFUNCTION()
	virtual TArray<struct FInputParameter> ReturnInputParameter() override;
	UPROPERTY()
	TArray<FInputParameter> InputParameters;

	// Input Function
	UFUNCTION()
	void Input_StartFlying(const FInputActionValue& Value);

	UFUNCTION()
	void Input_StopFlying(const FInputActionValue& Value);

	// JetPack Energy 관리
	void ManageJetPackEnergy(bool Flying);

	// JetPack Cool 관리
	void ManageJetPackCoolTime();
	
	float CheckCoolTime = 0.f;
	
private:
	// 초기 설정
	void InitSettings();

	// Flying 상태 변경
	void SetIsFlying(bool Flying);

	// Character Move 바꾸고 Interface 통해 JetPack 애니메이션 실행
	void ToggleFlight();

	// Tick마다 캐릭터 이동 및 방향 변수 업데이트
	void SetInputParam();

	// 쿨타임 시작
	void StartCooling(float DeltaTime);

public:
	// Blueprint에서 처리하는 함수
	UFUNCTION(BlueprintImplementableEvent)
	void InitBPSetting(UAnimInstance* PlayerAnimIns);

	// Flying 가능 상태 Mesh
	UFUNCTION(BlueprintImplementableEvent)
	void SciJetpackEquip(bool IsEquip);

	// Flying
	UFUNCTION(BlueprintImplementableEvent)
	void SciJetpackFlying(UAnimInstance* PlayerAnimIns, bool Flying);

	// 입력에 따른 애니메이션
	UFUNCTION(BlueprintImplementableEvent)
	void SciJetpackInputs(UAnimInstance* PlayerAnimIns, float Forward, float Right, float Turn, float Up);
};
