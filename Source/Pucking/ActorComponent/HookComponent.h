// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/BindInputInterface.h"
#include "Interfaces/EquipInterface.h"
#include "Interfaces/FireInterface.h"
#include "HookComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PUCKING_API UHookComponent : public UActorComponent, public IFireInterface, public IEquipInterface, public IBindInputInterface
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
	// 갈고리 발사 애님 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* HookShootMontage;

	// 갈고리 Timer
	UPROPERTY()
	FTimerHandle HookTimer;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HookComponent SkeletalMesh")
	USkeletalMesh* HookSkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HookComponent SkeletalMeshComponent")
	USkeletalMeshComponent* HookSkeletalMeshComponent;

public:
	// Owner에 갈고리 장착
	UFUNCTION(BlueprintCallable)
	virtual void Equip(USkeletalMeshComponent* TargetSkeletalMeshComp, FName SocketName, FTransform ActorTransform) override;
	
	// 갈고리 발사
	UFUNCTION(BlueprintCallable)
	virtual void Fire(FVector StartLoc, FVector ForwardVector) override;

	// Input Event
	UFUNCTION()
	void Input_Hook();

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HookComponent Properties")
	float HookRange = 1000.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HookComponent Properties")
	float HookCoolTime = 3.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HookComponent Properties")
	bool IsCanHookShoot = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HookComponent Properties")
	class UCableComponent* CableComponent;

private:
	UFUNCTION()
	void AttachCableToActor(AActor* Actor, FVector HitLocation);
	
};
