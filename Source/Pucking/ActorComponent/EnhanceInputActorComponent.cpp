// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/EnhanceInputActorComponent.h"
#include "Common/CommonStruct.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values for this component's properties
UEnhanceInputActorComponent::UEnhanceInputActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnhanceInputActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEnhanceInputActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEnhanceInputActorComponent::BindInput(UEnhancedInputLocalPlayerSubsystem* Subsystem, UEnhancedInputComponent* EnhancedInputComponent, struct FInputParameter& InputParameter)
{
	if(!Subsystem) return;
	if(!EnhancedInputComponent) return;
	UE_LOG(LogTemp, Warning, TEXT("String : %s"), *InputParameter.CallbackFunc.ToString());
	Subsystem->AddMappingContext(InputParameter.InputMappingContext, 1);
	EnhancedInputComponent->BindAction(InputParameter.InputAction, InputParameter.TriggerEvent, InputParameter.TargetClass, InputParameter.CallbackFunc);
}