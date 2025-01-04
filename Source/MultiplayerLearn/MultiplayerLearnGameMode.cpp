// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerLearnGameMode.h"
#include "MultiplayerLearnCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultiplayerLearnGameMode::AMultiplayerLearnGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
