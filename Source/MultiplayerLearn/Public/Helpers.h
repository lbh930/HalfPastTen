// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class MULTIPLAYERLEARN_API Helpers
{
public:
	Helpers();
	~Helpers();

	static void PrintString(const FString& str) {
		if (GEngine) {
			FString formattedMessage = FString::Printf(TEXT("[Client %d] %s"), UE::GetPlayInEditorID(), *str);
			GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Green, formattedMessage);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *formattedMessage);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("GEngine is null when trying to print: %s"), *str);
		}
	}
};
