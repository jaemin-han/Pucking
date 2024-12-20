// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/HP_ManagementComponent.h"
#include "ActorComponent/StatusComponent.h"

// Sets default values for this component's properties
UHP_ManagementComponent::UHP_ManagementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHP_ManagementComponent::BeginPlay()
{
	Super::BeginPlay();
	UStatusComponent* Status = GetOwner()->FindComponentByClass<UStatusComponent>();
	CurrentHP = Status->MaxHP;
	// ...
	
}


// Called every frame
void UHP_ManagementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DrawDebugString(GetWorld(), GetOwner()->GetActorLocation() - FVector(0, 0, 20), FString::Printf(TEXT("HP : %.1f"), CurrentHP), 0, FColor::Red, 0.005f, false, 2.0f);
	// ...
}

void UHP_ManagementComponent::GetDamage(float damage)
{
	UStatusComponent* Status = GetOwner()->FindComponentByClass<UStatusComponent>();
	damage = damage - Status->DefensePoint;

	CurrentHP -= damage;
}

