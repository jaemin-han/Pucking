// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "Interfaces/BindInputInterface.h"
#include "Interfaces/EquipInterface.h"
#include "HookComponent.generated.h"

enum class ECharacterMontage : uint8;
class IMontageFSMInterface;
class AGrapHookMesh;

UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PUCKING_API UHookComponent : public UActorComponent, public IEquipInterface, public IBindInputInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHookComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 초기화
	virtual void InitActorComponent();

public:
	// 갈고리 발사 준비애님 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hook AnimMontage")
	UAnimMontage* HookModeMontage;
	
	// 갈고리 발사 애님 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hook AnimMontage")
	UAnimMontage* HookShootMontage;

	// 몽타주 재생할 때 필요한 Owner 정보
	IMontageFSMInterface* OwnerFsmInterface;

	// 캐릭터 무브먼트
	UPROPERTY()
	class UCharacterMovementComponent* OwnerMovement;

	// 카메라 스프링암
	UPROPERTY()
	class USpringArmComponent* PlayerSpringArmComponent;

	// 갈고리 Timer
	UPROPERTY()
	FTimerHandle HookTimer;
	
	// FOnHookCoolTime Delegate;
	TMulticastDelegate<void(float)> OnHookCoolDelegate;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HookComponent SkeletalMesh")
	USkeletalMesh* HookSkeletalMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HookComponent SkeletalMeshComponent")
	USkeletalMeshComponent* HookSkeletalMeshComponent;
	
	/*UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HookComponent StaticMesh")
	UStaticMesh* HookStaticMesh;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HookComponent StaticMeshComponent")
	UStaticMeshComponent* HookMeshComponent;*/

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HookComponent SkeletalMesh Propertiy")
	FName AttachedSocketName = "hand_l";

public:
	// Owner에 갈고리 장착
	UFUNCTION(BlueprintCallable)
	virtual void Equip(USkeletalMeshComponent* TargetSkeletalMeshComp, FName SocketName, FTransform ActorTransform) override;
	
	// 갈고리 발사
	UFUNCTION(BlueprintCallable)
	virtual void ShootHook(FVector StartLoc, FVector ForwardVector);

	// Input Event
	UFUNCTION()
	void Input_HookMode();

	UFUNCTION()
	void Input_HookShoot();

	// Return Input Bind Parameter
	virtual TArray<FInputParameter> ReturnInputParameter() override;

public:
	// 총 Input 관련 FInputParameter 구조체
	UPROPERTY()
	TArray<FInputParameter> InputParameters;

	// Owner에게 추가해줄 InputMappingContext
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hook InputMappingContext")
	class UInputMappingContext* HookInputMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hook InputAction")
	class UInputAction* HookInputAction;

public:
	// 날아가는 힘
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HookComponent Properties")
	float LaunchRate = 1500.f;

	// 갈고리로 이동할 수 있는 사정거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HookComponent Properties")
	float HookRange = 2000.f;

	// 갈고리로 이동할 수 있는 최소 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HookComponent Properties")
	float MinCanHook = 500.f;

	// 갈고리 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HookComponent Properties")
	float HookCoolTime = 3.f;

	// 갈고리 상태일 때 카메라 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HookComponent Properties")
	float HookModeSpringArmLength = 80.f;

	// 검지하는 구체 반지름
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HookComponent Properties")
	float HookTraceRadius = 20.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HookComponent Properties")
	bool IsCanHookShoot = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HookComponent Properties")
	class UCableComponent* CableComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HookComponent Properties")
	float OriginSpringArmLength;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HookComponent Properties")
	bool IsHookCool = false;

	// 갈고리 가능 프리뷰 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HookComponent Properties")
	TSubclassOf<AActor> HookPreviewClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "HookComponent Properties")
	AActor* HookPreviewActor;

	UPROPERTY()
	float CheckCoolTime = 0.f;

	// JetpackUI
	UPROPERTY()
	class USubCoolTimeUI* SubCoolTimeUI;

private:
	UPROPERTY()
	float OriginGravity;

	UPROPERTY()
	float OriginAirControl;

	UPROPERTY()
	float OriginGroundFriction;
	
private:
	UFUNCTION()
	void InitCableComponent();
	
	UFUNCTION()
	void LaunchToCable(const FVector& HitLocation);

	bool CheckCanHook(ECharacterMontage HookFsmMontage);

	// Timeline
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HookComponent Timeline Properties", meta = (AllowPrivateAccess = "true"))
	UCurveFloat* CurveFloat;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HookComponent Timeline Properties", meta = (AllowPrivateAccess = "true"))
	UTimelineComponent* HookTimelineComponent;

	UPROPERTY()
	FOnTimelineFloat TimelineEvent;
	
	UPROPERTY()
	FOnTimelineEvent EndTimelineEvent;
	
	UFUNCTION()
	void StartHookTimer(float Value);

	UFUNCTION()
	void EndHookTimer();

	UPROPERTY()
	FVector DestinationVector;
	
	bool bIsHitActor = false;

	///////////////////////////////////////////////////////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hook AnimMontage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* StartMontage;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hook AnimMontage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* EndMontage;*/
	
	UFUNCTION()
	void OnHookMontageStartCallback(UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnHookMontageEnd(UAnimMontage* Montage, bool bInterrupted);

	FOnMontageBlendingOutStarted BlendingOutDelegate;
	FOnMontageBlendingOutStarted EndDelegate;
};
