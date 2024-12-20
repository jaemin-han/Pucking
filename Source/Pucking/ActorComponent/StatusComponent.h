// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatusComponent.generated.h"



UENUM(BlueprintType)
enum class EDamageType : uint8
{
	Physical UMETA(DisplayName = "DamageType_Physical"),
	Fire   UMETA(DisplayName = "DamageType_Fire"),
	Ice    UMETA(DisplayName = "DamageType_Ice")
};

UENUM(BlueprintType)
enum class EOptionType : uint8
{
	MaxHP,
	PhysicalDF,
	FireDF,
	IceDF,
	Shield,
	Dmg,
	CriticalC,
	CriticalM,
	PhysicalPen,
	FirePen,
	IcePen

};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PUCKING_API UStatusComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatusComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP_Status")
	float MaxHP = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense_Status")
	float PhysicalDefense = 29;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense_Status")
	float FireDefense = 28;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defense_Status")
	float IceDefense = 27;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shield_Status")
	float MaxShield = 500;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage_Status")
	float Damage = 30;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage_Status")
	float CriticalChance = 0.1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage_Status")
	float CriticalMultipier = 1.2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage_Status")
	float PhysicalPenetration = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage_Status")
	float FirePenetration = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage_Status")
	float IcePenetration = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage_Status")
	EDamageType DamageType = EDamageType::Physical;
	
public:
	UFUNCTION(BlueprintCallable)
	void ApplyOption(float number, EOptionType optionType);
};
