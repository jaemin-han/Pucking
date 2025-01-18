// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimComponent.h"

#include "InputTriggers.h"
#include "Common/CommonStruct.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values for this component's properties
UAnimComponent::UAnimComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	WalkSpeed = 400.0f;
	JogSpeed = 600.0;
	bIsJogging = false;
	bIsIronSight = false;
}


// Called when the game starts
void UAnimComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ACharacter>(GetOwner());

	if (WalkAndJogBlendSpace)
	{
		SetBlendSpaceSpeeds(WalkSpeed, JogSpeed);
	}

	// IronSightBlendSpace 가 있다면
	if (IronSightBlendSpace)
	{
		// IronSightBlendSpace 의 Sample 중 이름에 Walk가 들어있으면 Y 값을 WalkSpeed 로 설정
		auto& BlendSamples = const_cast<TArray<FBlendSample>&>(IronSightBlendSpace->GetBlendSamples());

		for (auto& Sample : BlendSamples)
		{
			if (Sample.Animation.GetName().Contains("Walk"))
			{
				Sample.SampleValue.Y = WalkSpeed;
			}
		}

		IronSightBlendSpace->ResampleData();
	}
}

void UAnimComponent::SetBlendSpaceSpeeds(float NewWalkSpeed, float NewJogSpeed)
{
	// Owner 의 speed 를 NewWalkSpeed 로 설정
	Owner->GetCharacterMovement()->MaxWalkSpeed = NewWalkSpeed;

	auto& BlendSamples = const_cast<TArray<FBlendSample>&>(WalkAndJogBlendSpace->GetBlendSamples());

	for (auto& Sample : BlendSamples)
	{
		// Sample 의 이름에 "Walk" 가 포함되어 있다면 Y 값에 NewWalkSpeed 를 설정
		if (Sample.Animation.GetName().Contains("Walk"))
		{
			Sample.SampleValue.Y = NewWalkSpeed;
		}
		else if (Sample.Animation.GetName().Contains("Jog"))
		{
			Sample.SampleValue.Y = NewJogSpeed;
		}
	}

	WalkAndJogBlendSpace->ResampleData();
}

void UAnimComponent::HandleStartJog()
{
	Owner->GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	bIsJogging = true;

	// IronSight 중이라면 IronSight 를 끝내고 Jog 를 시작
	HandleEndIronSight();
}

void UAnimComponent::HandleEndJog()
{
	Owner->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bIsJogging = false;
}

TArray<struct FInputParameter> UAnimComponent::ReturnInputParameter()
{
	if (AnimInputMappingContext)
	{
		// Jog
		if (JogAction)
		{
			// started
			FInputParameter JogInputParameter;

			JogInputParameter.TargetClass = this;
			JogInputParameter.TriggerEvent = ETriggerEvent::Started;
			JogInputParameter.InputMappingContext = AnimInputMappingContext;
			JogInputParameter.InputAction = JogAction;
			JogInputParameter.CallbackFunc = FName("HandleStartJog");

			InputParameters.Push(JogInputParameter);

			// ended
			FInputParameter EndJogInputParameter;

			EndJogInputParameter.TargetClass = this;
			EndJogInputParameter.TriggerEvent = ETriggerEvent::Completed;
			EndJogInputParameter.InputMappingContext = AnimInputMappingContext;
			EndJogInputParameter.InputAction = JogAction;
			EndJogInputParameter.CallbackFunc = FName("HandleEndJog");

			InputParameters.Push(EndJogInputParameter);
		}

		// IronSight
		if (IronSightAction)
		{
			// started
			FInputParameter IronSightInputParameter;

			IronSightInputParameter.TargetClass = this;
			IronSightInputParameter.TriggerEvent = ETriggerEvent::Started;
			IronSightInputParameter.InputMappingContext = AnimInputMappingContext;
			IronSightInputParameter.InputAction = IronSightAction;
			IronSightInputParameter.CallbackFunc = FName("HandleStartIronSight");

			InputParameters.Push(IronSightInputParameter);

			// ended
			FInputParameter EndIronSightInputParameter;

			EndIronSightInputParameter.TargetClass = this;
			EndIronSightInputParameter.TriggerEvent = ETriggerEvent::Completed;
			EndIronSightInputParameter.InputMappingContext = AnimInputMappingContext;
			EndIronSightInputParameter.InputAction = IronSightAction;
			EndIronSightInputParameter.CallbackFunc = FName("HandleEndIronSight");

			InputParameters.Push(EndIronSightInputParameter);
		}
	}

	return InputParameters;
}

// Called every frame
void UAnimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Owner)
		return;
}

void UAnimComponent::HandleStartIronSight()
{
	// Jog 중이라면 Jog 를 끝내고 IronSight 를 시작
	HandleEndJog();
	bIsIronSight = true;
}

void UAnimComponent::HandleEndIronSight()
{
	bIsIronSight = false;
}

void UAnimComponent::HandleWeaponType(EWeaponType InWeaponType)
{
	CurWeaponType = InWeaponType;
}
