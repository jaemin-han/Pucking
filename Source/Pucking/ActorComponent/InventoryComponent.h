// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/ItemData.h"
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

public:
	TMap<int32, FItemData> ItemDataMap;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	class AItemBase* InteractingItem;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	ACharacter* Owner;

	UPROPERTY(VisibleAnywhere, Category = "Item")
	class UCameraComponent* InvenCameraComponent;

public:
	// 부착된 엑터의 EnhancedInput 을 세팅하는 함수
	void SetEnhancedInput();
	// 상호작용 중인 아이템을 처리하는 함수, Owner의 EnhancedInputComponent 에 binding
	void HandleInteractingItem();
	// InteractingItem을 감지하는 함수
	void DetectInteractingItem();
};
