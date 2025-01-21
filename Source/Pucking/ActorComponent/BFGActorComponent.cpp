// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/BFGActorComponent.h"

#include "InputTriggers.h"
#include "AOE/ProjectileBase.h"

UBFGActorComponent::UBFGActorComponent()
{
	WeaponType = EWeaponType::BFG;
}

void UBFGActorComponent::Fire(FVector StartLoc, FVector ForwardVector)
{
	Super::Fire(StartLoc, ForwardVector);
	FActorSpawnParameters SpawnParameters;
	GetWorld()->SpawnActor<AProjectileBase>(BFGProjectileClass, StartLoc, FRotator::ZeroRotator, SpawnParameters);
}

TArray<struct FInputParameter> UBFGActorComponent::ReturnInputParameter()
{
	Super::ReturnInputParameter();
	// Rifle Input 함수
	if(GunInputMappingContext)
	{
		// 초기화
		InputParameters.Empty();
		
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
	}
	return InputParameters;
}

void UBFGActorComponent::Input_Fire(const FInputActionValue& Value)
{
	Super::Input_Fire(Value);

	// 사격 불가능 상태면 return;
	if(!bIsShootAble) return;
	
	// 사격 애님몽타주 재생
	PlayOwnerMontage(BFGFireMontage, 1.f);
}