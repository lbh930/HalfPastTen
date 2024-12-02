// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

void PrintString(const FString& str) {
	if (GEngine) {
		FString formattedMessage = FString::Printf(TEXT("[Client %d] %s"), UE::GetPlayInEditorID(), *str);
		GEngine->AddOnScreenDebugMessage(-1, 30.0f, FColor::Green, formattedMessage);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("GEngine is null when trying to print: %s"), *str);
	}
}


UMultiplayerSessionSubsystem::UMultiplayerSessionSubsystem():mSessionName(FName("Half Past Ten")) {
	PrintString("UMultiplayerSessionSubsystem::Constructor");
	bCreateServerAfterDestroy = false;
	DestroyServerName = FString("");
	ServerNameToFind = FString("");

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

			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this,
				&UMultiplayerSessionSubsystem::OnJoinSessionComplete);
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
	PrintString("UMultiplayerSessionSubsystem::CreateServer");

	if (ServerName.IsEmpty()) {
		PrintString("ServerName is empty");
		ServerCreateDel.Broadcast(false);
		return;
	}

	FNamedOnlineSession *ExistingSession = SessionInterface->GetNamedSession(mSessionName);
	if (ExistingSession) {
		//Session already exists
		bCreateServerAfterDestroy = true;
		DestroyServerName = ServerName;
		PrintString("Session already exists, destroying it");
		SessionInterface->DestroySession(mSessionName);
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
		PrintString("OnlineSubsystem is not valid");
	}

	//print SERVER_NAME to be set
	PrintString(FString::Printf(TEXT("Setting ServerName: %s"), *ServerName));
	SessionSettings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, mSessionName, SessionSettings);
}

void UMultiplayerSessionSubsystem::FindServer(FString ServerName) {
	PrintString("UMultiplayerSessionSubsystem::FindServer");

	if (ServerName.IsEmpty()) {
		PrintString("ServerName is empty");
		ServerJoinDel.Broadcast(false);
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

	ServerNameToFind = ServerName;
	PrintString(FString::Printf(TEXT("Setting ServerNameToFind as: %s"), *ServerName));

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UMultiplayerSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) {
	PrintString(FString::Printf(TEXT("UMultiplayerSessionSubsystem::OnCreateSessionComplete: %d"), bWasSuccessful));

	ServerCreateDel.Broadcast(bWasSuccessful);

	if (bWasSuccessful) {
		PrintString("Session created successfully");
		GetWorld()->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
	}
	else {
		PrintString("Session creation failed");
	}
}

void UMultiplayerSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful) {
	PrintString(FString::Printf(TEXT("UMultiplayerSessionSubsystem::OnCreateSessionComplete: %d"), bWasSuccessful));

	if (bWasSuccessful) {
		PrintString("Session Destroyed successfully");

		if (bCreateServerAfterDestroy) {
			bCreateServerAfterDestroy = false;
			CreateServer(DestroyServerName);
		}
	}
	else {
		PrintString("Session Destroy failed");
	}
}

void UMultiplayerSessionSubsystem::OnFindSessionsComplete(bool bWasSuccessful) {
	PrintString(FString::Printf(TEXT("UMultiplayerSessionSubsystem::OnFindSessionsComplete: %d"), bWasSuccessful));

	if (ServerNameToFind.IsEmpty()) {
		PrintString("ServerNameToFind is empty");
		ServerJoinDel.Broadcast(false);
		return;
	}

	if (bWasSuccessful) {
		PrintString("Session found successfully");

		FOnlineSessionSearchResult CorrectResult;
		bool bFound = false;

		if (SessionSearch.IsValid()) {
			PrintString("found: " + FString::FromInt(SessionSearch->SearchResults.Num()) + " sessions");
			for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++) {
				FOnlineSessionSearchResult& SearchResult = SessionSearch->SearchResults[i];
				if (SearchResult.IsValid()) {
					PrintString("Session found: " + SearchResult.GetSessionIdStr());
					FString ServerName = "No-name";

					//print all values
					for (const auto& Setting : SearchResult.Session.SessionSettings.Settings) {
						FString KeyName = Setting.Key.ToString();
						FString Value;
						Setting.Value.Data.GetValue(Value);
						PrintString(FString::Printf(TEXT("Key: %s, Value: %s"), *KeyName, *Value));
					}

					//SearchResult.Session.SessionSettings.
					SearchResult.Session.SessionSettings.Get(FName("SERVER_NAME"), ServerName);

					FString Msg2 = FString::Printf(TEXT("ServerName: %s"), *ServerName);
					PrintString(Msg2);

					if (ServerName.Equals(ServerNameToFind)) {
						CorrectResult = SearchResult;
						FString Msg3 = FString::Printf(TEXT("Found correct server: %s"), *ServerName);
						PrintString(Msg3);
						bFound = true;
						break;
					}
				}
				else {
					PrintString("Session is not valid");
				}
			}

			if (bFound) {
				SessionInterface->JoinSession(0, mSessionName, CorrectResult);
			}
			else {
				PrintString(FString::Printf(TEXT("Couldn't find server: %s"), *ServerNameToFind));
				ServerNameToFind = "";
				ServerJoinDel.Broadcast(false);
			}
		}
		else {
			PrintString("SessionSearch is not valid");
			ServerJoinDel.Broadcast(false);
		}
	}
	else {
		PrintString("Session found failed");
		ServerJoinDel.Broadcast(false);
	}
}

void UMultiplayerSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
	PrintString(FString::Printf(TEXT("UMultiplayerSessionSubsystem::OnJoinSessionComplete: %d"), Result));

	if (Result == EOnJoinSessionCompleteResult::Success) {
		PrintString(FString::Printf(TEXT("OnJoinSessionComplete: Join session success - %s"), *mSessionName.ToString()));
		
		FString Address = "";
		bool Success = SessionInterface->GetResolvedConnectString(mSessionName, Address);
		if (Success) {
			PrintString(FString::Printf(TEXT("OnJoinSessionComplete: Address: %s"), *Address));
			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController) {
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
			else {
				PrintString("OnJoinSessionComplete: PlayerController is null");
				ServerJoinDel.Broadcast(false);
			}
		}
		else {
			PrintString("OnJoinSessionComplete: GetResolvedConnectString failed");
			ServerJoinDel.Broadcast(false);
		}
	}
	else {
		PrintString("OnJoinSessionComplete: Join session failed");
		ServerJoinDel.Broadcast(false);
	}
}