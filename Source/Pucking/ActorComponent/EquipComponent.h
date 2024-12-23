// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/CommonEnum.h"
#include "EquipComponent.generated.h"

// TMap 과 TArray 를 사용하기 위해
USTRUCT(Blueprintable, BlueprintType)
struct FItemSlotArray
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ItemSlotArray")
	TArray<class UItemSlot*> ItemSlots;
};


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

	// WeaponItemSlots
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EquipComponent")
	TMap<EWeaponType, FItemSlotArray> WeaponItemSlots;


public:
	// WeaponItemSlots 의 FItemSlotArray 에 ItemSlot 을 추가하는 함수
	UFUNCTION()
	void AddItemSlot(EWeaponType WeaponType, class UItemSlot* ItemSlot);

	// 다른 컴포넌트에서 사용할 함수
	// 특정 WeaponType, 특정 총알 Index 를 가지는 ItemSlot 을 리턴하는 Getter
	class UItemSlot* GetItemSlot(EWeaponType WeaponType, int32 AmmoIndex);

	// 다른 컴포넌트에서 사용할 함수
	// 특정 WeaponType, 특정 총알 Index 를 가지는 ItemSlot 의 ItemOptions 를 리턴하는 Getter
	// TArray<class UOptionDataAsset*> ItemOptions;
	TArray<class UOptionDataAsset*> GetItemOptions(EWeaponType WeaponType, int32 AmmoIndex);
	
};
