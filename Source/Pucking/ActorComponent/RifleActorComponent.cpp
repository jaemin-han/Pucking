// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/RifleActorComponent.h"

#include "InputTriggers.h"
#include "Camera/CameraComponent.h"
#include "CameraShake/RifleCameraShake.h"
#include "GameFramework/Character.h"

URifleActorComponent::URifleActorComponent()
{
}

void URifleActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// Rifle Struct 데이터 세팅
	SetDefaultGunInfoStruct(TEXT("Rifle"));
}

void URifleActorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URifleActorComponent::Fire(FVector StartLoc, FVector ForwardVector)
{
	// 끝 위치 = 시작 위치에다가 (전방방향 * 총의 사격범위)를 더함
	FVector EndLoc = StartLoc + ForwardVector * GunInfoStruct.Range;
	
	FHitResult _hitRes;

	FCollisionQueryParams _collisionParam;
	_collisionParam.AddIgnoredActor(GetOwner());

	//Y, Z 방향의 반동
	EndLoc.Y += FMath::RandRange(GunInfoStruct.SpreadY * -1, GunInfoStruct.SpreadY);
	EndLoc.Z += FMath::RandRange(GunInfoStruct.SpreadZ * -1, GunInfoStruct.SpreadZ);
	
	bool isHit = GetWorld()->LineTraceSingleByChannel(_hitRes, StartLoc, EndLoc, ECC_Pawn, _collisionParam);
	DrawDebugLine(GetWorld(), StartLoc, EndLoc, FColor::Green, true, 5.f);
	
	if(isHit)
	{
		if(AActor* hitActor = _hitRes.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor is : %s"), *hitActor->GetName());
		}
	}
	
	GunInfoStruct.Magazine--;
	
	CameraShakeRecoil();

	Super::Fire(StartLoc, ForwardVector);
}

void URifleActorComponent::Reload()
{
	SetIsShootAble(false);
	
	if(RifleReloadMontage && OwnerCharacter)
	{
		if(UAnimInstance* OwnerAnimIns = OwnerCharacter->GetMesh()->GetAnimInstance())
		{
			OwnerAnimIns->Montage_Play(RifleReloadMontage);
		}
	}
	
	Super::Reload();
}

void URifleActorComponent::SetSpreadRange(float Y, float Z)
{
	Super::SetSpreadRange(Y, Z);
}

void URifleActorComponent::CameraShakeRecoil()
{
	Super::CameraShakeRecoil();

	//카메라 반동
	GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(URifleCameraShake::StaticClass());
}

TArray<struct FInputParameter> URifleActorComponent::ReturnInputParameter()
{
	// Rifle Input 함수
	if(GunInputMappingContext)
	{
		// Fire
		if(FireInputAction)
		{
			FInputParameter FireInputParameter;
			
			FireInputParameter.TargetClass = this;
			FireInputParameter.TriggerEvent = ETriggerEvent::Triggered;
			FireInputParameter.InputMappingContext = GunInputMappingContext;
			FireInputParameter.InputAction = FireInputAction;
			FireInputParameter.CallbackFunc = FName("Input_Fire");

			InputParameters.Add(FireInputParameter);
		}

		// Reload
		if(ReloadInputAction)
		{
			// Reload Input 함수
			FInputParameter ReloadInputParameter;
	
			ReloadInputParameter.TargetClass = this;
			ReloadInputParameter.TriggerEvent = ETriggerEvent::Started;
			ReloadInputParameter.InputMappingContext = GunInputMappingContext;
			ReloadInputParameter.InputAction = ReloadInputAction;
			ReloadInputParameter.CallbackFunc = FName("Reload");

			InputParameters.Add(ReloadInputParameter);
		}
	}
	
	return InputParameters;
}

void URifleActorComponent::Input_Fire(const FInputActionValue& Value)
{
	Super::Input_Fire(Value);

	// 사격 불가능 상태면 return;
	if(!bIsShootAble) return;
	
	// 남은 총알 확인
	if(GunInfoStruct.Magazine <= 0)
	{
		//Reload();
		return;
	}

	if(RifleFireMontage && OwnerCharacter)
	{
		/*if(SkeletalMeshComponent && SkeletalMeshComponent->GetAnimInstance())
		{
			SkeletalMeshComponent->GetAnimInstance()->Montage_Play(FireAnimMontage);
		}*/
		if(UAnimInstance* OwnerAnimIns = OwnerCharacter->GetMesh()->GetAnimInstance())
		{
			OwnerAnimIns->Montage_Play(RifleFireMontage);
		}
	}
	
	FVector OriginStartLoc = OwnerCameraComp->GetComponentLocation();
	
	OriginStartLoc.X +=  GetOwner()->GetActorLocation().X - OriginStartLoc.X;
	OriginStartLoc.Y += - 40;
	
	Fire(OriginStartLoc, OwnerCameraComp->GetForwardVector());
}
