// Fill out your copyright notice in the Description page of Project Settings.


#include "PuckAnimInstance.h"

#include "ActorComponent/AnimComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPuckAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(TryGetPawnOwner());
}

void UPuckAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Owner)
	{
		UE_LOG(LogTemp, Error, TEXT("Owner is nullptr"));
		return;
	}

	if (!AnimComponent)
	{
		AnimComponent = Owner->FindComponentByClass<UAnimComponent>();
		if (!AnimComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("AnimComponent is nullptr"));
			return;
		}
	}

	// speed
	FVector Velocity = Owner->GetVelocity();
	FVector LateralVelocity = FVector(Velocity.X, Velocity.Y, 0.f);
	Speed = LateralVelocity.Size();

	// ZSpeed
	ZSpeed = Velocity.Z;

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

	// bIsFalling
	bIsFalling = Owner->GetCharacterMovement()->IsFalling();

	// bIsIronSight
	bIsIronSight = AnimComponent->bIsIronSight;

	// Pitch
	FRotator AimRotation = Owner->GetBaseAimRotation();
	Pitch = AimRotation.Pitch;
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
