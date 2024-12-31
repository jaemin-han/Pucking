// Fill out your copyright notice in the Description page of Project Settings.


#include "PuckAnimInstance.h"

#include "ActorComponent/AnimComponent.h"
#include "GameFramework/Character.h"

void UPuckAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(TryGetPawnOwner());
	if (Owner)
	{
		AnimComponent = Owner->FindComponentByClass<UAnimComponent>();
	}
}

void UPuckAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Owner)
		return;

	// speed
	FVector Velocity = Owner->GetVelocity();
	FVector LateralVelocity = FVector(Velocity.X, Velocity.Y, 0.f);
	Speed = LateralVelocity.Size();

	GEngine->AddOnScreenDebugMessage(123123, 0.1f, FColor::Black, FString::Printf(TEXT("Speed : %f"), Speed));
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
	GEngine->AddOnScreenDebugMessage(1231234, 0.1f, FColor::Black, FString::Printf(TEXT("Direction : %f"), Direction));
}

// todo: 부정확한 코드입니다. 왼쪽으로 갈 때 -90 이랑 270 이 나왔다 안나왔다 합니다. 수정하세요!!
float UPuckAnimInstance::CalculateDirection(FVector Velocity, FRotator BaseRotation)
{
	FVector ForwardVector = FRotationMatrix(BaseRotation).GetUnitAxis(EAxis::X);
	FVector2D Velocity2D = FVector2D(Velocity.X, Velocity.Y).GetSafeNormal();
	FVector2D ForwardVector2D = FVector2D(ForwardVector.X, ForwardVector.Y).GetSafeNormal();

	float AngleRadians = FMath::Atan2(ForwardVector2D.X, ForwardVector2D.Y) - FMath::Atan2(Velocity2D.X, Velocity2D.Y);
	float AngleDegrees = FMath::RadiansToDegrees(AngleRadians);
	return AngleDegrees;
}
