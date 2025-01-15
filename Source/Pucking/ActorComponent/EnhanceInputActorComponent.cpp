// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent/EnhanceInputActorComponent.h"
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

// 모든 InputAction Binding
void UEnhanceInputActorComponent::BindInput(UEnhancedInputLocalPlayerSubsystem* Subsystem, UEnhancedInputComponent* EnhancedInputComponent, struct FInputParameter& InputParameter)
{
	if(!Subsystem) return;
	if(!EnhancedInputComponent) return;
	
	Subsystem->AddMappingContext(InputParameter.InputMappingContext, 1);
	EnhancedInputComponent->BindAction(InputParameter.InputAction, InputParameter.TriggerEvent, InputParameter.TargetClass, InputParameter.CallbackFunc);
}

// 특정 InputAction 다시 Bind
void UEnhanceInputActorComponent::ActivateMappingContext(UEnhancedInputLocalPlayerSubsystem* Subsystem, UEnhancedInputComponent* EnhancedInputComponent, struct FInputParameter& InputParameter)
{	
	// 다시 Mapping	
	EnhancedInputComponent->BindAction(InputParameter.InputAction, InputParameter.TriggerEvent, InputParameter.TargetClass, InputParameter.CallbackFunc);
}

// Bind 된 특정 InputAction 삭제
void UEnhanceInputActorComponent::DeactivateMappingContext(UEnhancedInputLocalPlayerSubsystem* Subsystem, UEnhancedInputComponent* EnhancedInputComponent, struct FInputParameter& InputParameter)
{
	// 전체 EnhancedInputComponent를 돌면서 매개변수로 넘어온 InputAction만 삭제
	const TArray<TUniquePtr<FEnhancedInputActionEventBinding>>& Bindings = EnhancedInputComponent->GetActionEventBindings();
	for(int32 i = 0; i < Bindings.Num(); i++)
	{
		if(Bindings[i]->GetAction() == InputParameter.InputAction)
		{
			EnhancedInputComponent->RemoveActionEventBinding(i);
		}
	}
}
