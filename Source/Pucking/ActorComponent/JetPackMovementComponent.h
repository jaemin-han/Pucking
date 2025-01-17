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
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsFlying = false;

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
	
public:
	UFUNCTION()
	virtual void Equip(USkeletalMeshComponent* TargetSkeletalMeshComp, FName SocketName, FTransform ActorTransform) override;
	
	UFUNCTION()
	virtual TArray<struct FInputParameter> ReturnInputParameter() override;

	UPROPERTY()
	TArray<FInputParameter> InputParameters;

	UFUNCTION()
	void Input_StartFlying(const FInputActionValue& Value);

	UFUNCTION()
	void Input_StopFlying(const FInputActionValue& Value);
	
public:
	// 초기 설정
	void InitSettings();

	// Flying 상태 변경
	void SetIsFlying(bool Flying);

	// Character Move 바꾸고 Interface 통해 JetPack 애니메이션 실행
	void ToggleFlight();

	// Tick마다 캐릭터 이동 및 방향 변수 업데이트
	void SetInputParam();

public:
	// Blueprint에서 처리하는 함수
	UFUNCTION(BlueprintImplementableEvent)
	void InitBPSetting(UAnimInstance* PlayerAnimIns);

	UFUNCTION(BlueprintImplementableEvent)
	void SciJetpackFlying(UAnimInstance* PlayerAnimIns, bool Flying);

	UFUNCTION(BlueprintImplementableEvent)
	void SciJetpackInputs(UAnimInstance* PlayerAnimIns, float Forward, float Right, float Turn, float Up);
	
};
