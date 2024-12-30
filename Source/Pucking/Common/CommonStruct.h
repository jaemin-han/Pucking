// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonEnum.h"
#include "CommonStruct.generated.h"

/**
 * 
 */

enum class ETriggerEvent : uint8;

USTRUCT(BlueprintType)
struct FGunInfoStruct : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType GunType = EWeaponType::Rifle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DefaultDamage = 0;

	// 현재 총알
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Magazine = 0;

	// 한 탄창의 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxMagazine = 0;

	// Y축 탄 퍼짐
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpreadY = 0;

	// Z축 탄 퍼짐
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpreadZ = 0;

	// Pitch 카메라 반동
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoilPitch = 0;

	// Yaw 카메라 반동
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoilYaw = 0;

	// 총 사정거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range = 0;

	// 총 연사율
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootInterval = 0;
};

USTRUCT(BlueprintType)
struct FInputParameter
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UObject* TargetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETriggerEvent TriggerEvent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UInputAction* InputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CallbackFunc;
}; 

class PUCKING_API CommonStruct
{
public:
	CommonStruct();
	~CommonStruct();
};
