// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/CommonStruct.h"
#include "EnhanceInputActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUCKING_API UEnhanceInputActorComponent : public UActorComponent//, public IBindInputInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEnhanceInputActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION()
	void BindInput(UEnhancedInputLocalPlayerSubsystem* Subsystem, UEnhancedInputComponent* EnhancedInputComponent, struct FInputParameter& InputParameter);

	UFUNCTION()
	void ActivateMappingContext(UEnhancedInputLocalPlayerSubsystem* Subsystem, UEnhancedInputComponent* EnhancedInputComponent, struct FInputParameter& InputParameter);
	
	UFUNCTION()
	void DeactivateMappingContext(UEnhancedInputLocalPlayerSubsystem* Subsystem, UEnhancedInputComponent* EnhancedInputComponent, struct FInputParameter& InputParameter);
};
