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

	// X축 탄 퍼짐
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpreadX = 0;

	// Y축 탄 퍼짐
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpreadY = 0;

	// Z축 탄 퍼짐
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpreadZ = 0;

	/*// Pitch 카메라 반동
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoilPitch = 0;

	// Yaw 카메라 반동
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RecoilYaw = 0;*/

	// 총 사정거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range = 0;

	// 총 연사율
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ShootInterval = 0;

	// 라이플 UI 반동 - 캐릭터 최대 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerMaxSpd = 0;

	// 라이플 UI 반동 - 캐릭터 속도 비례 보정값
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxUISpreadPerSpd = 0;

	// 라이플 UI 반동 - 사격 비례 보정값 최대치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxUISpreadPerFire = 0;

	// Zoom 했을 때 반동값이 보정되는 정도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ModifyZoomRecoil = 0.2f;
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

// 라이플 강화 옵션
USTRUCT(BlueprintType)
struct FRifleSkillParameter : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DecreaseSpreadX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DecreaseSpreadY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DecreaseSpreadZ;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ModifyZoomRecoil;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DecreaseUISpreadPerSpd;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DecreaseUISpreadPerFire;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 IncreaseMaxMagazine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SetReloadAnimRate;
};

// 샷건 강화 옵션
USTRUCT(BlueprintType)
struct FShotgunSkillParameter : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SetShootInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IncreaseBulletNum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 IncreaseMaxMagazine;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SetReloadAnimRate;
};

USTRUCT(BlueprintType)
struct FAnimMontageManage : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* AnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECharacterMontage CharacterMontage;
};

// Skill Data
USTRUCT(BlueprintType)
struct FSkillData : public FTableRowBase
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText WidgetText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* SkillIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText PopupTitle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText PopupContent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 RequiredEssence;
};


class PUCKING_API CommonStruct
{
public:
	CommonStruct();
	~CommonStruct();
};
