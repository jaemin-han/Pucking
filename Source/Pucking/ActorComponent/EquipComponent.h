// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/CommonEnum.h"
#include "EquipComponent.generated.h"

struct FInputActionValue;
// EWeaponType 을 입력으로 받는 delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponTypeChanged, EWeaponType, WeaponType);
// AmmoIndex 가 변경될 때 호출할 delegate
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAmmoIndexChanged);
// Status Component 를 변경시킬 때 호출할 delegate
// signature 로 EWeaponType, AmmoIndex 를 받아서 호출
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatusComponentChanged, EWeaponType, WeaponType, int32, AmmoIndex);

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

	// CurWeaponType 을 변경하는 InputAction
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ChangeWeaponTypeAction;

	// CurAmmoIndex 를 변경하는 InputAction
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ChangeAmmoIndexAction;

	// todo: debug 용 Reload InputAction
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ReloadAction;

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

	// CurWeaponType 을 변경하는 함수
	UFUNCTION()
	void HandleWeaponType(const FInputActionValue& Value);

	// CurAmmoIndex 를 변경하는 함수
	UFUNCTION()
	void HandleAmmoIndex(const FInputActionValue& Value);

	// todo: InventoryComponent 와 연결해서 창을 열고 닫는 방식으로 수정해야함
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

	// MainHUD class
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "EquipComponent")
	TSubclassOf<class UMainHUD> MainHUDClass;

	// MainHUD instance
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EquipComponent")
	class UMainHUD* MainHUD;

	// WeaponItemSlots
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EquipComponent")
	TMap<EWeaponType, FItemSlotArray> WeaponItemSlotMap;

	// EWeaponType 별로 어떤 AmmoIndex 를 사용할지 저장하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EquipComponent")
	TMap<EWeaponType, int32> WeaponAmmoIndexMap;

	// 현재 선택된 WeaponType 과 AmmoIndex 를 저장하는 변수
	EWeaponType CurWeaponType;
	int32 CurAmmoIndex;

	// delegate
	UPROPERTY(BlueprintAssignable, Category = "EquipComponent")
	FOnWeaponTypeChanged OnWeaponTypeChanged;
	UPROPERTY(BlueprintAssignable, Category = "EquipComponent")
	FOnAmmoIndexChanged OnAmmoIndexChanged;
	UPROPERTY(BlueprintAssignable, Category = "EquipComponent")
	FOnStatusComponentChanged OnStatusComponentChanged;

public:
	// WeaponItemSlots 의 FItemSlotArray 에 ItemSlot 을 추가하는 함수
	UFUNCTION()
	void AddItemSlot(EWeaponType InWeaponType, class UItemSlot* ItemSlot);

	// 다른 컴포넌트에서 사용할 함수
	// 특정 WeaponType, 특정 총알 Index 를 가지는 ItemSlot 을 리턴하는 Getter
	class UItemSlot* GetItemSlot(EWeaponType InWeaponType, int32 InAmmoIndex);

	// 다른 컴포넌트에서 사용할 함수
	// 특정 WeaponType, 특정 총알 Index 를 가지는 ItemSlot 의 ItemOptions 를 리턴하는 Getter
	// TArray<class UOptionDataAsset*> ItemOptions;
	TArray<class UOptionDataAsset*> GetItemOptions(EWeaponType InWeaponType, int32 InAmmoIndex);
	
	// todo: WeaponComponent 에서 사용할 함수
	// 사용할 수 있는 총알을 리턴해주고, 사용한 총알을 제거하는 함수
	int32 OnReload(int32 MagazineCapacity);
	// todo: debug 용 OnReload 함수
	void DebugOnReload();

	UFUNCTION()
	void ApplyToMainHUD();
};
