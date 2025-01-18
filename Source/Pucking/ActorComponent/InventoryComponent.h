// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/BindInputInterface.h"
#include "Item/ItemInstanceData.h"
#include "InventoryComponent.generated.h"

DECLARE_DELEGATE(FInventoryOnOffDelegate);


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PUCKING_API UInventoryComponent : public UActorComponent, public IBindInputInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

private:
	// InventoryGrid class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UInventoryGrid> InventoryGridClass;
	// InventoryGrid instance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UInventoryGrid* InventoryGrid;
	// ItemSlot class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UItemSlot> ItemSlotClass;

public:
#pragma region IBindInputInterface
	UPROPERTY()
	TArray<FInputParameter> InputParameters;

	// MappingContext
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* ItemMappingContext;

	// item interaction input action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ItemInteractionAction;

	// InventoryOnOff input action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InventoryOnOffAction;

	virtual TArray<struct FInputParameter> ReturnInputParameter() override;

#pragma endregion

	UPROPERTY(VisibleAnywhere, Category = "Item")
	class APickableItem* InteractingItem;

	UPROPERTY(VisibleAnywhere, Category = "Owner")
	ACharacter* Owner;

	UPROPERTY(VisibleAnywhere, Category = "Owner")
	class UCameraComponent* OwnerCameraComponent;

	// player controller
	UPROPERTY(VisibleAnywhere, Category = "Owner")
	class APlayerController* OwnerPlayerController;

	// UItemSlot Array
	UPROPERTY(VisibleAnywhere, Category = "Item")
	TArray<class UItemSlot*> ItemSlotArray;

public:
	// 상호작용 중인 아이템을 처리하는 함수, Owner의 EnhancedInputComponent 에 binding
	UFUNCTION()
	void HandleInteractingItem();
	// InteractingItem을 감지하는 함수
	void DetectInteractingItem();
	// todo: EquipComponent 와 연결해서 창을 열고 닫는 방식으로 수정해야함
	// IA_InventoryOnOff 를 처리하는 함수
	UFUNCTION()
	void HandleInventoryOnOff();

	// GetFirstAmmoItemSlot
	// argument 로 WeaponType, DamageType 을 받아서 가장 먼저 발견되는 AmmoItemSlot 을 반환
	// class UAmmoItemSlot* GetFirstAmmoItemSlot(EWeaponType WeaponType, EDamageType DamageType);

	// EquipComponent 와 연결해서 창을 열고 닫는 delegate
	FInventoryOnOffDelegate InventoryOnOffDelegate;
};
