// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Common/CommonEnum.h"
#include "EquipComponent.h"
#include "StatusComponent.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUCKING_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* StatusMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* StatusOnOffAction;

public:	
	// Sets default values for this component's properties
	UStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UPlayerStatusWidget> PlayerStatusWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI", meta = (AllowPrivateAccess = "true"))
	class UPlayerStatusWidget* PlayerStatusWidget;


public:

	UPROPERTY(VisibleAnywhere)
	ACharacter* Owner;
	UPROPERTY(VisibleAnywhere)
	class APlayerController* OwnerPlayerController;


	//변동하지 않는 수치(영구적으로 오를때만 변경)
	//다른곳에서 사용할 때는 이 변수이름은 가능하면 사용X 밑의 Cur붙은 변수들 사용
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP_Status")
	float MaxHP = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense_Status")
	float PhysicalDefense = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense_Status")
	float FireDefense = 25;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense_Status")
	float IceDefense = 20;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield_Status")
	float MaxShield = 500;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage_Status")
	float Damage = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage_Status")
	float CriticalChance = 0.1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage_Status")
	float CriticalMultipier = 1.2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage_Status")
	float PhysicalPenetration = 5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage_Status")
	float FirePenetration = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage_Status")
	float IcePenetration = 15;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage_Status")
	EDamageType CommonDamageType = EDamageType::Physical;
	
	//무기로 인한 변동에 사용하는 스텟
	//다른 곳에서 사용해야 할 때 이 변수 이름 사용
	UPROPERTY(EditAnywhere)
	float CurIcePenetration;
	UPROPERTY(EditAnywhere)
	float CurFirePenetration;
	UPROPERTY(EditAnywhere)
	float CurPhysicalPenetration;
	UPROPERTY(EditAnywhere)
	float CurCriticalMultipier;
	UPROPERTY(EditAnywhere)
	float CurCriticalChance;
	UPROPERTY(EditAnywhere)
	float CurDamage;
	UPROPERTY(EditAnywhere)
	float CurMaxShield;
	UPROPERTY(EditAnywhere)
	float CurMaxHP;
	UPROPERTY(EditAnywhere)
	float CurPhysicalDefense;
	UPROPERTY(EditAnywhere)
	float CurFireDefense;
	UPROPERTY(EditAnywhere)
	float CurIceDefense;

	//스테이터스를 종합하기 위해 가져야 할 다른 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Need_Components")
	class UEquipComponent* EquipComp;

	UPROPERTY()
	TArray<class UOptionDataAsset*> GetDataAssetArray;
	UPROPERTY()
	TArray<class UOptionDataAsset*> CurrentDataAssetArray;

	EWeaponType CurrentWeaponType;
	float CurrentAmmoIndex;

public:
	UFUNCTION(BlueprintCallable)
	void ApplyOption(EWeaponType WeaponType, int32 AmmoIndex);
	//void ApplyOption(float number, EOptionType optionType);
	UFUNCTION(BlueprintCallable)
	void IncreaseOption(EOptionType OptionType, float OptionValue);
	UFUNCTION(BlueprintCallable)
	void ResetStaticStatus();

	void SetEnhancedInput();
	void StatusOnOff();
};
