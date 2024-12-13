// Fill out your copyright notice in the Description page of Project Settings.

#include "HalfPastTenPlayer.h"
#include "HandCard.h"
#include "Net/UnrealNetwork.h"
#include "Helpers.h"
#include "SeatManager.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"


AHalfPastTenPlayer::AHalfPastTenPlayer()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

void AHalfPastTenPlayer::BeginPlay()
{
	Super::BeginPlay();

	//Get the seat manager in the scene
	TArray<AActor*> FoundSeatManagers;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASeatManager::StaticClass(), FoundSeatManagers);
	if (FoundSeatManagers.Num() > 0)
	{
		SeatManager = Cast<ASeatManager>(FoundSeatManagers[0]);
	}
	else {
		Helpers::PrintString("No SeatManager found in the scene");
	}
    
    if (IsLocallyControlled()) {
        APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
        if (PlayerController)
        {
            PlayerController->bShowMouseCursor = true;
            PlayerController->DefaultMouseCursor = EMouseCursor::Default;
        }
    }
    
    if (IsLocallyControlled())
    {
        APlayerController* PlayerController = Cast<APlayerController>(GetController());
        if (PlayerController)
        {
            PlayerController->bShowMouseCursor = true;
            PlayerController->DefaultMouseCursor = EMouseCursor::Default;
        }else{
            Helpers::PrintString("AHalfPastTenLogic::BeginPlay() - PlayerController is nullptr");
        }
    }
}


double AHalfPastTenPlayer::GetTotalCardValues()
{
	double sum = 0;
	for (int i = 0; i < PlayerCardValues.Num(); i++) {
		if (PlayerCardValues[i] <= 10 && PlayerCardValues[i] >= 1) {
			sum += double(PlayerCardValues[i]);
		}
		else if (PlayerCardValues[i] > 10 && PlayerCardValues[i] <= 13) {
			sum += 0.5;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("PlayerCards[%d] has an invalid value"), i);
		}
	}
	return sum;
}

void AHalfPastTenPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHalfPastTenPlayer, PlayerCardValues);
	DOREPLIFETIME(AHalfPastTenPlayer, PlayerCardFaceUp);
	DOREPLIFETIME(AHalfPastTenPlayer, bReady);
}

void AHalfPastTenPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    
	if (HandDeck) {
		HandDeck->SetActorLocation(FVector(0, 0, HandDeckHeight));
		FVector ActorForwardVector = GetActorForwardVector();
		ActorForwardVector.Z = 0;
		HandDeck->SetActorLocation(GetActorLocation() + ActorForwardVector * HandDeckDistance);
		FRotator actorRotation = GetActorRotation();
		actorRotation.Pitch = 0;
		actorRotation.Roll = 0;
		actorRotation.Yaw += 90;
		HandDeck->SetActorRotation(actorRotation);
		if (IsLocallyControlled()) {
			HandDeck->SetCardValues(PlayerCardValues, PlayerCardFaceUp, true);
		}
		else {
			HandDeck->SetCardValues(PlayerCardValues, PlayerCardFaceUp, false);
		}
		
	}
	else {
		Helpers::PrintString("HandDeck is not set for player");
	}
}

FString AHalfPastTenPlayer::GetPlayerReadyText() {
	//Set Readyness text
	if (SeatManager) {
		FString readyText = "";
		TArray<APlayerBase*> PlayerBases = SeatManager->GetPlayers();

		//Helpers::PrintString("PlayerBases.Num(): " + FString::FromInt(PlayerBases.Num()));

		PlayerBases.Sort([](const APlayerBase& LHS, const APlayerBase& RHS) {
			return LHS.PlayerId < RHS.PlayerId;
			});

		for (int i = 0; i < PlayerBases.Num(); i++) {
			if (PlayerBases[i] == nullptr) {
				Helpers::PrintString("AHalfPastTenPlayer::Tick() - PlayerBases[] is nullptr at " + FString::FromInt(i));
				continue;
			}
			AHalfPastTenPlayer* Player = Cast<AHalfPastTenPlayer>(PlayerBases[i]);
			if (Player) {
				readyText += FString::Printf(TEXT("Player %d: %s\n"), Player->PlayerId, Player->bReady ? TEXT("Ready") : TEXT("Not Ready"));
			}
			else {
				Helpers::PrintString("AHalfPastTenPlayer::Tick() - Players[] is nullptr at " + FString::FromInt(i));
			}
		}

		//Helpers::PrintString("ready text: " + readyText);

		return readyText;
	}
	else {
		Helpers::PrintString("SeatManager is not set for player");
	}
	return "";
}

void AHalfPastTenPlayer::ServerSetReady_Implementation(bool bNewReady)
{
	if (HasAuthority()) {
		this->bReady = bNewReady;
	}
}


void AHalfPastTenPlayer::UpdatePlayerWidgetText(UTextBlock* textBlock, const FString& newText)
{
	if (textBlock)
	{
		// Set the text
		textBlock->SetText(FText::FromString(newText));
	}
	else
	{
		Helpers::PrintString("TextBlock is null");
	}
}

void AHalfPastTenPlayer::DealCard(int cardValue, bool bIsFaceUp)
{
	PlayerCardValues.Add(cardValue);
	PlayerCardFaceUp.Add(bIsFaceUp);
}
