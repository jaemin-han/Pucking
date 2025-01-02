// Fill out your copyright notice in the Description page of Project Settings.


#include "InputTriggers.h"
#include "ShotGunActorComponent.h"
#include "Camera/CameraComponent.h"
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

void UShotgunActorComponent::Fire(FVector StartLoc, FVector ForwardVector)
{
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

	Super::Fire(StartLoc, ForwardVector);
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

	//GetWorld()->GetFirstPlayerController()->PlayerCameraManager->StartCameraShake(URifleCameraShake::StaticClass());
}

TArray<struct FInputParameter> UShotgunActorComponent::ReturnInputParameter()
{
	// Rifle Input 함수
	if(GunInputMappingContext)
	{
		// Fire
		if(FireInputAction)
		{
			FInputParameter FireInputParameter;
			
			FireInputParameter.TargetClass = this;
			FireInputParameter.TriggerEvent = ETriggerEvent::Started;
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

void UShotgunActorComponent::Input_Fire(const FInputActionValue& Value)
{
	Super::Input_Fire(Value);

	FVector OriginStartLoc = OwnerCameraComp->GetComponentLocation();
	
	OriginStartLoc.X +=  GetOwner()->GetActorLocation().X - OriginStartLoc.X;
	OriginStartLoc.Y += - 40;
	
	Fire(OriginStartLoc, OwnerCameraComp->GetForwardVector());
}
