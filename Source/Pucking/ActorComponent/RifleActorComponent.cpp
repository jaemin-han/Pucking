// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/RifleActorComponent.h"

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

void URifleActorComponent::Equip(USkeletalMeshComponent* TargetSkeletalMeshComp, FName SocketName,
	FTransform ActorTransform)
{
	Super::Equip(TargetSkeletalMeshComp, SocketName, ActorTransform);

	if(GunStaticMesh)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);

		if (UStaticMeshComponent* StaticMeshComponent = NewObject<UStaticMeshComponent>(TargetSkeletalMeshComp->GetOwner()))
		{
			// StaticMesh 정보를 StaticMeshComponent에 
			StaticMeshComponent->SetStaticMesh(GunStaticMesh);

			// SkeletalMesh에 붙이기
			StaticMeshComponent->AttachToComponent(TargetSkeletalMeshComp, AttachmentRules, SocketName);

			// Component 등록하기
			StaticMeshComponent->RegisterComponent();
		}
	}
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

	// TODO 추후 구조 정해지면 적절한 곳으로 옮겨야함
	GunInfoStruct.Magazine--;

	// TODO 매개변수로 흔들림 조절할 수 있게 변경 필요
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
