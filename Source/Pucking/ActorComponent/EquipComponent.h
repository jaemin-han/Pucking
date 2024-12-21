// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EquipComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PUCKING_API UEquipComponent : public UActorComponent
{
	GENERATED_BODY()

	// MappingContext
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* EquipMappingContext;

	// EquipWidget OnOff input action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* EquipOnOffAction;

public:
	// Sets default values for this component's properties
	UEquipComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 부착된 엑터의 EnhancedInput 을 세팅하는 함수
	void SetEnhancedInput();

	// EquipWidget 을 On/Off 하는 함수
	UFUNCTION()
	void HandleEquipOnOff();

public:
	UPROPERTY(VisibleAnywhere, Category = "Owner")
	ACharacter* Owner;

	// OwnerPlayerController
	UPROPERTY(VisibleAnywhere, Category = "Owner")
	class APlayerController* OwnerPlayerController;

	// EquipWidget class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EquipComponent")
	TSubclassOf<class UEquipWidget> EquipWidgetClass;

	// EquipWidget instance
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "EquipComponent")
	class UEquipWidget* EquipWidget;
};
