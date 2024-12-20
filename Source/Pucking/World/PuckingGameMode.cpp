// Copyright Epic Games, Inc. All Rights Reserved.

#include "PuckingGameMode.h"
#include "Pucking/Character/PuckingCharacter.h"
#include "UObject/ConstructorHelpers.h"

APuckingGameMode::APuckingGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
