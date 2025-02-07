// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MontageFSMInterface.generated.h"

enum class ECharacterFSM : uint8;
enum class ECharacterMontage : uint8;
// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMontageFSMInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PUCKING_API IMontageFSMInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool CheckChangeStateByMontage(ECharacterMontage TargetMontageState) = 0;
	virtual bool CheckChangeStateByFsm(ECharacterFSM TargetFsm) = 0;
	
	virtual void ReceiveMontageState(ECharacterMontage TargetMontageState, float InRate = 1) = 0;
	virtual void ReceiveFsm(ECharacterFSM TargetFsm) = 0;
};
