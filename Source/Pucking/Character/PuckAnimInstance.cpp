// Fill out your copyright notice in the Description page of Project Settings.


#include "PuckAnimInstance.h"

#include "ActorComponent/AnimComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPuckAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(TryGetPawnOwner());
	IsJetPackActive = false;
}

void UPuckAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	FString CrntState = UEnum::GetValueAsString(CurrentFSM);
	//GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Red, FString::Printf(TEXT("%s"), *CrntState), true);
	if (!Owner)
	{
		// UE_LOG(LogTemp, Error, TEXT("Owner is nullptr"));
		return;
	}

	if (!AnimComponent)
	{
		AnimComponent = Owner->FindComponentByClass<UAnimComponent>();
		if (!AnimComponent)
		{
			// UE_LOG(LogTemp, Error, TEXT("AnimComponent is nullptr"));
			return;
		}
	}

	// speed
	FVector Velocity = Owner->GetVelocity();
	FVector LateralVelocity = FVector(Velocity.X, Velocity.Y, 0.f);
	Speed = LateralVelocity.Size();

	// ZSpeed
	ZSpeed = Velocity.Z;

	// GEngine->AddOnScreenDebugMessage(123123, 0.1f, FColor::Black, FString::Printf(TEXT("Speed : %f"), Speed));
	// Direction
	// Speed 가 아주 작은 값이라면 Direction = 0.0f
	if (Speed < 1.0f)
	{
		Direction = 0.0f;
	}
	else
	{
		Direction = CalculateDirection(Velocity, Owner->GetActorRotation());
	}

	// bIsFalling
	bIsFalling = Owner->GetCharacterMovement()->IsFalling();

	// bIsIronSight
	bIsIronSight = AnimComponent->bIsIronSight;

	// Pitch
	FRotator AimRotation = Owner->GetBaseAimRotation();
	Pitch = AimRotation.Pitch;

	// CurWeaponType
	CurWeaponType = AnimComponent->CurWeaponType;
}

float UPuckAnimInstance::CalculateDirection(FVector Velocity, FRotator BaseRotation)
{
	FVector ForwardVector = FRotationMatrix(BaseRotation).GetUnitAxis(EAxis::X);
	FVector2D Velocity2D = FVector2D(Velocity.X, Velocity.Y).GetSafeNormal();
	FVector2D ForwardVector2D = FVector2D(ForwardVector.X, ForwardVector.Y).GetSafeNormal();

	// ForwardVector2D 와 Velocity2D 의 내적을 구합니다.
	float DotProduct = FVector2D::DotProduct(ForwardVector2D, Velocity2D);
	// ForwardVector2D 와 Velocity2D 의 외적을 구합니다.
	float CrossProduct = FVector2D::CrossProduct(ForwardVector2D, Velocity2D);
	// 내적 값을 각도로 치환합니다. -1.0f ~ 1.0f 를 180.0f ~ 0.0f 으로 치환합니다.
	float Angle = FMath::Acos(DotProduct) * 180.0f / PI;
	// 외적 값이 0 보다 작다면 Angle 을 음수로 치환합니다.
	if (CrossProduct < 0.0f)
	{
		Angle *= -1.0f;
	}
	return Angle;
}

void UPuckAnimInstance::OnMontageEndEvent(UAnimMontage* TargetMontage, bool bInterrupted)
{
	//UE_LOG(LogTemp, Warning, TEXT("Montage : %s, bInterrupted : %d"), *TargetMontage->GetName(), bInterrupted);
	//CurrentFSM = ECharacterFSM::Idle;
}

bool UPuckAnimInstance::CheckChangeStateByMontage(ECharacterMontage TargetMontageState)
{
	ECharacterFSM TargetFsm = ChangeMontageToFsm(TargetMontageState);
	
	return CheckCanFsm(TargetFsm);
}

bool UPuckAnimInstance::CheckChangeStateByFsm(ECharacterFSM TargetFsm)
{
	return CheckCanFsm(TargetFsm);
}

/*bool UPuckAnimInstance::CheckChangeStateByMontage(ECharacterFSM TargetFSM)
{
	return CheckCanFsm(TargetFSM);
}*/

void UPuckAnimInstance::ReceiveMontageState(ECharacterMontage TargetMontageState, float InRate)
{
	ECharacterFSM TargetFsm = ChangeMontageToFsm(TargetMontageState);

	bool IsCanAction = CheckCanFsm(TargetFsm);
	
	if(IsCanAction)
	{
		//TODO
		StopPlayingFsm(TargetFsm);
		
		// 상태 저장 
		CurrentFSM = TargetFsm;
		
		// 상태에 맞는 AnimMontage를 찾고 실행
		FName KeyName = FName(StaticEnum<ECharacterMontage>()->GetNameStringByValue(static_cast<int64>(TargetMontageState)));
		if (AnimMontageTable)
		{
			FAnimMontageManage* DT_Montage = AnimMontageTable->FindRow<FAnimMontageManage>(FName(KeyName), TEXT(""));
			if(DT_Montage)
			{
				PlayAnimMontage(DT_Montage->AnimMontage, InRate);
			}
		}
	}
}

void UPuckAnimInstance::ReceiveFsm(ECharacterFSM TargetFsm)
{
	bool IsCanAction = CheckCanFsm(TargetFsm);
	
	if(IsCanAction)
	{
		StopPlayingFsm(TargetFsm);
		
		// 상태 저장 
		CurrentFSM = TargetFsm;
	}
}

void UPuckAnimInstance::StopPlayingFsm(ECharacterFSM NewFSM)
{
	if(CurrentFSM == ECharacterFSM::Zoom)
	{
		if(NewFSM == ECharacterFSM::Reloading || NewFSM == ECharacterFSM::Switching || NewFSM == ECharacterFSM::Hooking
			|| NewFSM == ECharacterFSM::HookMode)
		{
			Montage_Stop(0.25f, GetCurrentActiveMontage());
		}
	}
	else if(CurrentFSM == ECharacterFSM::JetpackMode)
	{
		if(NewFSM == ECharacterFSM::Hooking)
		{
			Montage_Stop(0.25f, GetCurrentActiveMontage());
		}
	}
	else if(CurrentFSM == ECharacterFSM::Hooking)
	{
		if(NewFSM == ECharacterFSM::JetpackMode)
		{
			Montage_Stop(0.25f, GetCurrentActiveMontage());
		}
	}
}

bool UPuckAnimInstance::CheckCanFsm(ECharacterFSM TargetFSM)
{
	bool bIsFsm = true;
	
	if(CurrentFSM == ECharacterFSM::Reloading)
	{
		if(TargetFSM == ECharacterFSM::Zoom || TargetFSM == ECharacterFSM::Switching
			|| TargetFSM == ECharacterFSM::HookMode || TargetFSM == ECharacterFSM::Hooking)
		{
			bIsFsm = false;
		}
	}
	else if(CurrentFSM == ECharacterFSM::Switching)
	{
		if(TargetFSM == ECharacterFSM::Fire || TargetFSM == ECharacterFSM::Reloading || TargetFSM == ECharacterFSM::Zoom
			|| TargetFSM == ECharacterFSM::HookMode)
		{
			bIsFsm = false;
		}
	}
	else if(CurrentFSM == ECharacterFSM::Hooking)
	{
		if(TargetFSM == ECharacterFSM::Reloading || TargetFSM == ECharacterFSM::Zoom || TargetFSM == ECharacterFSM::Switching
			|| TargetFSM == ECharacterFSM::JetpackMode)
		{
			bIsFsm = false;
		}
	}
	
	return bIsFsm;
}

// 몽타주 실행
void UPuckAnimInstance::PlayAnimMontage(UAnimMontage* Montage, float InRate)
{
	if(!Montage) return;
	
	if(!Montage_IsPlaying(Montage))
	{
		// 몽타주를 배속(InRate)으로 실행
		Montage_Play(Montage, InRate);
		
		blendOutDelegate.BindUObject(this, &UPuckAnimInstance::OnMontageEndEvent);
		Montage_SetBlendingOutDelegate(blendOutDelegate, Montage);
	}
}

// 몽타주를 캐릭터 상태 FSM으로
ECharacterFSM UPuckAnimInstance::ChangeMontageToFsm(ECharacterMontage TargetMontageState)
{
	ECharacterFSM ReturnFsm = CurrentFSM;
	
	if(TargetMontageState == ECharacterMontage::RifleFire || TargetMontageState == ECharacterMontage::ShotgunFire || TargetMontageState == ECharacterMontage::BFGFire)
	{
		// 사격
		ReturnFsm = ECharacterFSM::Fire;
	}
	else if(TargetMontageState == ECharacterMontage::RifleReload || TargetMontageState == ECharacterMontage::ShotgunReload || TargetMontageState == ECharacterMontage::BFGReload)
	{
		// 장전
		ReturnFsm = ECharacterFSM::Reloading;
	}
	else if(TargetMontageState == ECharacterMontage::Switching)
	{
		// 무기 교체
		ReturnFsm = ECharacterFSM::Switching;
	}
	else if(TargetMontageState == ECharacterMontage::Hooking)
	{
		// 그래플링 훅 날아가는 중
		ReturnFsm = ECharacterFSM::Hooking;
	}
	else if(TargetMontageState == ECharacterMontage::HookMode)
	{
		ReturnFsm = ECharacterFSM::HookMode;
	}
	else if(TargetMontageState == ECharacterMontage::JetpackMode)
	{
		ReturnFsm = ECharacterFSM::JetpackMode;
	} 

	return ReturnFsm;
}

bool UPuckAnimInstance::IsOnAir() const
{
	return (bIsFalling||IsJetPackActive) ;
}
