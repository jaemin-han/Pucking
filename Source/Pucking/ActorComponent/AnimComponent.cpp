// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimComponent.h"

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
	WalkSpeed = 350.0f;
	JogSpeed = 700.0f;
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

FInputParameter& UAnimComponent::ReturnInputParameter()
{
	return InputParameter;
}

// Called every frame
void UAnimComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!Owner)
		return;
}
