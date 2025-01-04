#include "MultiplayerSessionSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Helpers.h"

UMultiplayerSessionSubsystem::UMultiplayerSessionSubsystem()
	: mSessionName(FName("Half Past Ten"))
{
	Helpers::PrintString("UMultiplayerSessionSubsystem::Constructor");

	bCreateServerAfterDestroy = false;
	DestroyServerName = FString("");
	ServerNameToFind = FString("");
}

void UMultiplayerSessionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	Helpers::PrintString("UMultiplayerSessionSubsystem::Initialize");

	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		FString SubsystemName = OnlineSubsystem->GetSubsystemName().ToString();
		Helpers::PrintString("OnlineSubsystem: " + SubsystemName);

		SessionInterface = OnlineSubsystem->GetSessionInterface();
		if (SessionInterface.IsValid())
		{
			Helpers::PrintString("SessionInterface is valid");

			// 绑定委托
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
			Helpers::PrintString("SessionInterface is not valid");
		}
	}
	else
	{
		Helpers::PrintString("OnlineSubsystem is NULL");
	}
}

void UMultiplayerSessionSubsystem::Deinitialize()
{
	Super::Deinitialize();

	UE_LOG(LogTemp, Warning, TEXT("UMultiplayerSessionSubsystem::Deinitialize"));

	// 解除委托绑定，避免卸载后残留回调
	if (SessionInterface.IsValid())
	{
		SessionInterface->OnCreateSessionCompleteDelegates.RemoveAll(this);
		SessionInterface->OnDestroySessionCompleteDelegates.RemoveAll(this);
		SessionInterface->OnFindSessionsCompleteDelegates.RemoveAll(this);
		SessionInterface->OnJoinSessionCompleteDelegates.RemoveAll(this);
	}
}

void UMultiplayerSessionSubsystem::CreateServer(FString ServerName)
{
	Helpers::PrintString("UMultiplayerSessionSubsystem::CreateServer");

	// 先检查SessionInterface是否有效
	if (!SessionInterface.IsValid())
	{
		Helpers::PrintString("SessionInterface is invalid. Cannot create server.");
		ServerCreateDel.Broadcast(false);
		return;
	}

	if (ServerName.IsEmpty())
	{
		Helpers::PrintString("ServerName is empty");
		ServerCreateDel.Broadcast(false);
		return;
	}

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(mSessionName);
	if (ExistingSession)
	{
		// 如果会话已存在，先销毁再重建
		bCreateServerAfterDestroy = true;
		DestroyServerName = ServerName;
		Helpers::PrintString("Session already exists, destroying it");
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
	if (OnlineSubsystem)
	{
		FString SubsystemName = OnlineSubsystem->GetSubsystemName().ToString();
		if (SubsystemName == "Steam")
		{
			SessionSettings.bIsLANMatch = false;
		}
	}
	else
	{
		Helpers::PrintString("OnlineSubsystem is not valid");
	}

	// 设置自定义的 SERVER_NAME，以便后续查找/显示
	Helpers::PrintString(FString::Printf(TEXT("Setting ServerName: %s"), *ServerName));
	SessionSettings.Set(FName("SERVER_NAME"), ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, mSessionName, SessionSettings);
}

void UMultiplayerSessionSubsystem::FindServer(FString ServerName)
{
	Helpers::PrintString("UMultiplayerSessionSubsystem::FindServer");

	// 同样检查有效性
	if (!SessionInterface.IsValid())
	{
		Helpers::PrintString("SessionInterface is invalid. Cannot find server.");
		ServerJoinDel.Broadcast(false);
		return;
	}

	if (ServerName.IsEmpty())
	{
		Helpers::PrintString("ServerName is empty");
		ServerJoinDel.Broadcast(false);
		return;
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	// 默认启用 LAN 查询
	SessionSearch->bIsLanQuery = true;
	IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (OnlineSubsystem)
	{
		FString SubsystemName = OnlineSubsystem->GetSubsystemName().ToString();
		if (SubsystemName == "Steam")
		{
			SessionSearch->bIsLanQuery = false;
		}
	}

	// 查找带有 Presence 的会话
	SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	ServerNameToFind = ServerName;
	Helpers::PrintString(FString::Printf(TEXT("Setting ServerNameToFind as: %s"), *ServerName));

	SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
}

void UMultiplayerSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	Helpers::PrintString(FString::Printf(TEXT("UMultiplayerSessionSubsystem::OnCreateSessionComplete: %d"), bWasSuccessful));

	ServerCreateDel.Broadcast(bWasSuccessful);

	if (bWasSuccessful)
	{
		Helpers::PrintString("Session created successfully");
		// 主机端切换关卡
		GetWorld()->ServerTravel("/Game/Maps/HalfPastTen?listen");
	}
	else
	{
		Helpers::PrintString("Session creation failed");
	}
}

void UMultiplayerSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	// 修正打印的函数名
	Helpers::PrintString(FString::Printf(TEXT("UMultiplayerSessionSubsystem::OnDestroySessionComplete: %d"), bWasSuccessful));

	if (bWasSuccessful)
	{
		Helpers::PrintString("Session Destroyed successfully");

		// 如果销毁会话后还要重新建，自动重调 CreateServer
		if (bCreateServerAfterDestroy)
		{
			bCreateServerAfterDestroy = false;
			CreateServer(DestroyServerName);
		}
	}
	else
	{
		Helpers::PrintString("Session Destroy failed");
	}
}

void UMultiplayerSessionSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	Helpers::PrintString(FString::Printf(TEXT("UMultiplayerSessionSubsystem::OnFindSessionsComplete: %d"), bWasSuccessful));

	if (ServerNameToFind.IsEmpty())
	{
		Helpers::PrintString("ServerNameToFind is empty");
		ServerJoinDel.Broadcast(false);
		return;
	}

	if (bWasSuccessful)
	{
		Helpers::PrintString("Session found successfully");

		FOnlineSessionSearchResult CorrectResult;
		bool bFound = false;

		if (SessionSearch.IsValid())
		{
			Helpers::PrintString("found: " + FString::FromInt(SessionSearch->SearchResults.Num()) + " sessions");
			for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
			{
				FOnlineSessionSearchResult& SearchResult = SessionSearch->SearchResults[i];
				if (SearchResult.IsValid())
				{
					Helpers::PrintString("Session found: " + SearchResult.GetSessionIdStr());
					FString FoundServerName = "No-name";

					// 打印所有设置键值
					for (const auto& Setting : SearchResult.Session.SessionSettings.Settings)
					{
						FString KeyName = Setting.Key.ToString();
						FString Value;
						Setting.Value.Data.GetValue(Value);
						Helpers::PrintString(FString::Printf(TEXT("Key: %s, Value: %s"), *KeyName, *Value));
					}

					// 读取我们之前塞进去的 "SERVER_NAME"
					SearchResult.Session.SessionSettings.Get(FName("SERVER_NAME"), FoundServerName);

					Helpers::PrintString(FString::Printf(TEXT("ServerName: %s"), *FoundServerName));

					if (FoundServerName.Equals(ServerNameToFind))
					{
						CorrectResult = SearchResult;
						Helpers::PrintString(FString::Printf(TEXT("Found correct server: %s"), *FoundServerName));
						bFound = true;
						break;
					}
				}
				else
				{
					Helpers::PrintString("Session is not valid");
				}
			}

			if (bFound)
			{
				// 找到目标会话后加入
				SessionInterface->JoinSession(0, mSessionName, CorrectResult);
			}
			else
			{
				Helpers::PrintString(FString::Printf(TEXT("Couldn't find server: %s"), *ServerNameToFind));
				ServerNameToFind = "";
				ServerJoinDel.Broadcast(false);
			}
		}
		else
		{
			Helpers::PrintString("SessionSearch is not valid");
			ServerJoinDel.Broadcast(false);
		}
	}
	else
	{
		Helpers::PrintString("Session found failed");
		ServerJoinDel.Broadcast(false);
	}
}

void UMultiplayerSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	Helpers::PrintString(FString::Printf(TEXT("UMultiplayerSessionSubsystem::OnJoinSessionComplete: %d"), Result));

	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		Helpers::PrintString(FString::Printf(TEXT("OnJoinSessionComplete: Join session success - %s"), *mSessionName.ToString()));

		FString Address = "";
		bool Success = false;
		
		if (SessionInterface.IsValid())
		{
			Success = SessionInterface->GetResolvedConnectString(mSessionName, Address);
		}

		if (Success)
		{
			Helpers::PrintString(FString::Printf(TEXT("OnJoinSessionComplete: Address: %s"), *Address));
			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController)
			{
				PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			}
			else
			{
				Helpers::PrintString("OnJoinSessionComplete: PlayerController is null");
				ServerJoinDel.Broadcast(false);
			}
		}
		else
		{
			Helpers::PrintString("OnJoinSessionComplete: GetResolvedConnectString failed");
			ServerJoinDel.Broadcast(false);
		}
	}
	else
	{
		Helpers::PrintString("OnJoinSessionComplete: Join session failed");
		ServerJoinDel.Broadcast(false);
	}
}
