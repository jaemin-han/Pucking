// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/RifleActorComponent.h"

#include "InputTriggers.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

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
	Super::Fire(StartLoc, ForwardVector);

	// 남은 총알 확인
	if(GunInfoStruct.Magazine <= 0) return;

	// 사격 불가능 상태면 return;
	if(!bIsShootAble) return;
	
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
			UGameplayStatics::ApplyDamage(hitActor, GunInfoStruct.DefaultDamage, nullptr, nullptr, UDamageType::StaticClass());
		}
	}
	
	GunInfoStruct.Magazine--;
	
	CameraShakeRecoil();
	
	if(GetWorld())
	{
		// 발사 직후 사격 불가능 상태
		this->SetIsShootAble(false);

		// TimeManager를 통해 Delay 후 다시 사격 가능 상태
		FTimerHandle ShootAbleTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(ShootAbleTimerHandle, [this]()
		{
			this->SetIsShootAble(true);
		}, GunInfoStruct.ShootInterval, false);
	}

	if(MuzzleParticle)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleParticle, GetOwner()->GetActorLocation(), FRotator(0, 0, 0));	
	}
}

void URifleActorComponent::Reload()
{
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
	float PitchRecoil = FMath::RandRange(GunInfoStruct.RecoilPitch * -1, GunInfoStruct.RecoilPitch);
	float YawRecoill = FMath::RandRange(GunInfoStruct.RecoilYaw * -1, GunInfoStruct.RecoilYaw);

	if(GetOwner())
	{
		Cast<APawn>(GetOwner())->AddControllerPitchInput(PitchRecoil);
		Cast<APawn>(GetOwner())->AddControllerYawInput(YawRecoill);
	}
}

struct FInputParameter& URifleActorComponent::ReturnInputParameter()
{
	InputParameter.TargetClass = this;
	InputParameter.TriggerEvent = ETriggerEvent::Triggered;
	InputParameter.InputMappingContext = FireInputMappingContext;
	InputParameter.InputAction = FireInputAction;
	InputParameter.CallbackFunc = FName("Input_Fire");
	
	return InputParameter;
}

void URifleActorComponent::Input_Fire(const FInputActionValue& Value)
{
	FVector OriginStartLoc = OwnerCameraComp->GetComponentLocation();
	
	OriginStartLoc.X +=  GetOwner()->GetActorLocation().X - OriginStartLoc.X;
	OriginStartLoc.Y += - 40;
	
	Fire(OriginStartLoc, OwnerCameraComp->GetForwardVector());
}
