// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GetMagazineInterface.generated.h"

enum class EWeaponType : uint8;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGetMagazineInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PUCKING_API IGetMagazineInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 현재 무기 타입
	virtual EWeaponType GetWeaponType() = 0;
	
	// 전체 탄창 개수
	virtual int32 GetMaxMagazine() = 0;

	// 현재 탄 개수
	virtual int32 GetCurMagazine() = 0;
};
