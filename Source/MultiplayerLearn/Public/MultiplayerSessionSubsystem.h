// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Online/OnlineSessionNames.h"

#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionSubsystem.generated.h"
/**
 * 
 */
UCLASS()
class MULTIPLAYERLEARN_API UMultiplayerSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UMultiplayerSessionSubsystem();

	void Initialize(FSubsystemCollectionBase& Collection) override;
	void Deinitialize() override;


	IOnlineSessionPtr SessionInterface;

	UFUNCTION(BlueprintCallable)
	void CreateServer (FString ServerName);

	UFUNCTION(BlueprintCallable)
	void FindServer(FString ServerName);

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

	void OnFindSessionsComplete(bool bWasSuccessful);

private:
	bool bCreateServerAfterDestroy;
	FString DestroyServerName;
	TSharedPtr<FOnlineSessionSearch> SessionSearch;
};
