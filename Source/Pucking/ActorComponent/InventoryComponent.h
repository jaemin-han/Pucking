// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/ItemInstanceData.h"
#include "InventoryComponent.generated.h"



UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PUCKING_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	// MappingContext
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* ItemMappingContext;

	// item interaction input action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ItemInteractionAction;

	// InventoryOnOff input action
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* InventoryOnOffAction;

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
	TSubclassOf<class UUserWidget> InventoryGridClass;
	// InventoryGrid instance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UInventoryGrid* InventoryGrid;
	// ItemSlot class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UItemSlot> ItemSlotClass;

public:
	UPROPERTY(VisibleAnywhere, Category = "Item")
	TMap<FName, FItemInstanceData> ItemDataMap;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	class AItemBase* InteractingItem;



	UPROPERTY(VisibleAnywhere, Category = "Owner")
	ACharacter* Owner;
	
	UPROPERTY(VisibleAnywhere, Category = "Owner")
	class UCameraComponent* OwnerCameraComponent;

	// player controller
	UPROPERTY(VisibleAnywhere, Category = "Owner")
	class APlayerController* OwnerPlayerController;

public:
	// 부착된 엑터의 EnhancedInput 을 세팅하는 함수
	void SetEnhancedInput();
	// 상호작용 중인 아이템을 처리하는 함수, Owner의 EnhancedInputComponent 에 binding
	void HandleInteractingItem();
	// InteractingItem을 감지하는 함수
	void DetectInteractingItem();
	// IA_InventoryOnOff 를 처리하는 함수
	void HandleInventoryOnOff();
};
