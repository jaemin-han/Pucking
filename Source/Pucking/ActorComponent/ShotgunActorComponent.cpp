// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotGunActorComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UShotgunActorComponent::UShotgunActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	
}


// Called when the game starts
void UShotgunActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ShotGun Struct 데이터 세팅
	SetDefaultGunInfoStruct(TEXT("Shotgun"));
	
}


// Called every frame
void UShotgunActorComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                           FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Black, FString::Printf(TEXT("Shotgun ActorComponent Magazine is %d"), GunInfoStruct.Magazine));
}

void UShotgunActorComponent::Equip(USkeletalMeshComponent* TargetSkeletalMeshComp, FName SocketName, FTransform ActorTransform)
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

void UShotgunActorComponent::Fire(FVector StartLoc, FVector ForwardVector)
{
	Super::Fire(StartLoc, ForwardVector);

	// 남은 총알 확인
	if(GunInfoStruct.Magazine <= 0) return;

	// 사격 불가능 상태면 return;
	if(!bIsShootAble) return;

	for(int i=0; i < BulletNum; i++)
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
				UGameplayStatics::ApplyDamage(hitActor, GunInfoStruct.DefaultDamage, nullptr, nullptr, UDamageType::StaticClass());
			}
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

void UShotgunActorComponent::Reload()
{
	Super::Reload();
}

void UShotgunActorComponent::SetSpreadRange(float Y, float Z)
{
	Super::SetSpreadRange(Y, Z);
}

void UShotgunActorComponent::CameraShakeRecoil()
{
	Super::CameraShakeRecoil();

	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if(GetWorld() && OwnerPawn)
	{
		//float LocPitch = OwnerPawn->GetControlRotation().Pitch + GunInfoStruct.RecoilPitch;
		GetWorld()->GetTimerManager().SetTimer(RecoilTimerHandle, [OwnerPawn, this]()
		{
			if(elapsedTime > 1.f)
			{
				elapsedTime = 0.f;
				GetWorld()->GetTimerManager().ClearTimer(RecoilTimerHandle);
				return;
			}
			float CurrentPitch = OwnerPawn->GetControlRotation().Pitch;
			// 진행도 계산 (0.0 ~ 1.0)
			float Alpha = FMath::Clamp(elapsedTime, 0.0f, 1.0f);
			
			// 목표 피치로 부드럽게 보간
			//float TargetDelta = FMath::Lerp(CurrentPitch, LocPitch, Alpha);
			//UE_LOG(LogTemp, Warning, TEXT("TargetDelta : %f"), TargetDelta);
			
			OwnerPawn->AddControllerPitchInput(1);

			elapsedTime += 0.032;
			
		}, 0.032, true);
	}
}
