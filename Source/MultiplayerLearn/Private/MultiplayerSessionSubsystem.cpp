// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

void PrintString(const FString& str, UWorld* world) {
	if (GEngine) {
		FString formattedMessage = FString::Printf(TEXT("[Client %d] %s"), UE::GetPlayInEditorID(), *str);
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, formattedMessage);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("GEngine is null when trying to print: %s"), *str);
	}
}


void PrintString(const FString& str) {
	if (GEngine) {
		
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, str);
		
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("GEngine is null when trying to print: %s"), *str);
	}
}

UMultiplayerSessionSubsystem::UMultiplayerSessionSubsystem() {
	PrintString("UMultiplayerSessionSubsystem::Constructor");
	bCreateServerAfterDestroy = false;
	DestroyServerName = FString("");

}

void UMultiplayerSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection) {
	PrintString("UMultiplayerSessionSubsystem::Initialize");

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem) {
		FString subSysmtemName = OnlineSubsystem->GetSubsystemName().ToString();
		PrintString("OnlineSubsystem: " + subSysmtemName);

		SessionInterface = OnlineSubsystem -> GetSessionInterface();
		if (SessionInterface.IsValid()) {
			PrintString("SessionInterface is valid");

			//ADD DELEGATES
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, 
				&UMultiplayerSessionSubsystem::OnCreateSessionComplete);

			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this,
				&UMultiplayerSessionSubsystem::OnDestroySessionComplete);

			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this,
				&UMultiplayerSessionSubsystem::OnFindSessionsComplete);
		}	
		else
		{

			PrintString("SessionInterface is not valid");
		}
	}
	else {
		
	}
}

void UMultiplayerSessionSubsystem::Deinitialize() {
	UE_LOG(LogTemp, Warning, TEXT("UMultiplayerSessionSubsystem::Deinitialize"));
}

void UMultiplayerSessionSubsystem::CreateServer(FString ServerName) {
	PrintString("UMultiplayerSessionSubsystem::CreateServer", GetWorld());

	if (ServerName.IsEmpty()) {
		PrintString("ServerName is empty");
		return;
	}

	FName MySessionName = FName("Session Name");

	FNamedOnlineSession *ExistingSession = SessionInterface->GetNamedSession(MySessionName);
	if (ExistingSession) {
		//Session already exists
		bCreateServerAfterDestroy = true;
		DestroyServerName = ServerName;
		PrintString("Session already exists, destroying it", GetWorld());\
		SessionInterface->DestroySession(MySessionName);
		return;
	}

	FOnlineSessionSettings SessionSettings;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.bIsDedicated = false;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bUseLobbiesIfAvailable = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinViaPresence = true;
	SessionSettings.bIsLANMatch = true;

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem) {
		FString subSystemName = OnlineSubsystem->GetSubsystemName().ToString();
		if (subSystemName == "Steam") {
			SessionSettings.bIsLANMatch = false;
		}
	}
	else {
		PrintString("OnlineSubsystem is not valid", GetWorld());
	}

	SessionInterface->CreateSession(0, MySessionName, SessionSettings);
}

void UMultiplayerSessionSubsystem::FindServer(FString ServerName) {
	PrintString("UMultiplayerSessionSubsystem::FindServer", GetWorld());

	if (ServerName.IsEmpty()) {
		PrintString("ServerName is empty", GetWorld());
		return;
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	
	SessionSearch->bIsLanQuery = true;
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem) {
		FString subSystemName = OnlineSubsystem->GetSubsystemName().ToString();
		if (subSystemName == "Steam") {
			SessionSearch->bIsLanQuery = false;
		}
	}

	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UMultiplayerSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) {
	PrintString(FString::Printf(TEXT("UMultiplayerSessionSubsystem::OnCreateSessionComplete: %d"), bWasSuccessful), GetWorld());

	if (bWasSuccessful) {
		PrintString("Session created successfully", GetWorld());
		GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
	}
	else {
		PrintString("Session creation failed", GetWorld());
	}
}

void UMultiplayerSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful) {
	PrintString(FString::Printf(TEXT("UMultiplayerSessionSubsystem::OnCreateSessionComplete: %d"), bWasSuccessful), GetWorld());

	if (bWasSuccessful) {
		PrintString("Session Destroyed successfully", GetWorld());

		if (bCreateServerAfterDestroy) {
			bCreateServerAfterDestroy = false;
			CreateServer(DestroyServerName);
		}
	}
	else {
		PrintString("Session Destroy failed", GetWorld());
	}
}

void UMultiplayerSessionSubsystem::OnFindSessionsComplete(bool bWasSuccessful) {
	PrintString(FString::Printf(TEXT("UMultiplayerSessionSubsystem::OnFindSessionsComplete: %d"), bWasSuccessful));

	if (bWasSuccessful) {
		PrintString("Session found successfully", GetWorld());

		if (SessionSearch.IsValid()) {
			PrintString("found: " + FString::FromInt(SessionSearch->SearchResults.Num()) + " sessions", GetWorld());
			for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++) {
				FOnlineSessionSearchResult& SearchResult = SessionSearch->SearchResults[i];
				PrintString("Session found: " + SearchResult.GetSessionIdStr(), GetWorld());
			}
		}
		else {
			PrintString("SessionSearch is not valid", GetWorld());
		}
	}
	else {
		PrintString("Session found failed", GetWorld());
	}
}