// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Common/CommonEnum.h"
#include "Common/CommonStruct.h"
#include "Interfaces/MontageFSMInterface.h"
#include "PuckAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PUCKING_API UPuckAnimInstance : public UAnimInstance, public IMontageFSMInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	ACharacter* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	class UAnimComponent* AnimComponent;

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AnimMontage DataTable")
	UDataTable* AnimMontageTable;

	UPROPERTY()
	FAnimMontageManage AnimMontageStruct;

	virtual bool CheckChangeStateByMontage(ECharacterMontage TargetMontageState) override;
	virtual void ReceiveMontageState(ECharacterMontage TargetMontageState, float InRate) override;
	
	void StopPlayingFsm(ECharacterFSM NewFSM);
	bool CheckCanFsm(ECharacterFSM TargetFSM);
	void PlayAnimMontage(UAnimMontage* Montage, float InRate);

	ECharacterFSM ChangeMontageToFsm(ECharacterMontage TargetMontageState);

	bool IsOnAir() const;
protected:
	// speed of owner
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Speed;

	// direction of owner
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Direction;

	// pitch for aimoffset
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Pitch;

	// z value of velocity
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float ZSpeed;

	// is falling
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsFalling;

	// is iron sight
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsIronSight;

	// is JetPack
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool IsJetPackActive;

	// WeaponType
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	EWeaponType CurWeaponType;

	// CharacterFSM
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	ECharacterFSM CurrentFSM;

protected:
	float CalculateDirection(FVector Velocity, FRotator BaseRotation);
};
