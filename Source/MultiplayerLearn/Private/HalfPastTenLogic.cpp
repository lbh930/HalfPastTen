// Fill out your copyright notice in the Description page of Project Settings.


#include "HalfPastTenLogic.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Helpers.h"

// Sets default values
AHalfPastTenLogic::AHalfPastTenLogic():Super()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentState = EHalfPastTenGameState::GE_Start;
}

void AHalfPastTenLogic::BeginPlay()
{
	Super::BeginPlay();

	//get the seat manager
	SeatManager = Cast<ASeatManager>(UGameplayStatics::GetActorOfClass(GetWorld(), ASeatManager::StaticClass()));
	if (SeatManager == nullptr) {
		Helpers::PrintString("AHalfPastTenLogic::BeginPlay() - SeatManager not found");
	}

	RemainingDeck.Empty();
	RemainingDeck.Add(0);
	for (int i = 1; i <= 13; i++) {
		RemainingDeck.Add(4);
	}

}

int AHalfPastTenLogic::DrawCardFromDeck() {
	int cardCount = 0;
	for (int i = 0; i < RemainingDeck.Num(); i++) cardCount += RemainingDeck[i];
	//random an index from 1 to cardCount
	int randomIndex = FMath::RandRange(1, cardCount);
	int resCard = -1;
	for (int i = 0; i < RemainingDeck.Num(); i++) {
		if (randomIndex > RemainingDeck[i]) {
			randomIndex -= RemainingDeck[i];
		}
		else {
			RemainingDeck[i]--;
			resCard = i;
		}
	}
	return resCard;
}

void AHalfPastTenLogic::DealCardToPlayers(const TArray<AHalfPastTenPlayer*>& players) {
	if (HasAuthority()) {
		for (int playerIndex = 0; playerIndex < players.Num(); playerIndex++) {
			if (players[playerIndex] == nullptr) {
				Helpers::PrintString("AHalfPastTenLogic::DealCardToPlayers() - Player is nullptr at " + FString::FromInt(playerIndex));
				continue;
			}
			int card = this->DrawCardFromDeck();
			if (card == -1) {
				Helpers::PrintString("AHalfPastTenLogic::DealCardToPlayers() - No card left in deck");
				break;
			}
			players[playerIndex]->DealCard(card);
		}
	}
}

void AHalfPastTenLogic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority()) {
		switch (CurrentState)
		{
		case EHalfPastTenGameState::GE_Start:
			//UE_LOG(LogTemp, Warning, TEXT("Start Game"));
			break;
		case EHalfPastTenGameState::GS_Initial:
			//UE_LOG(LogTemp, Warning, TEXT("Initial (Every one get a card)"));

			//Check if all players has READY
		{
			TArray<AHalfPastTenPlayer*> players = SeatManager->GetHalfPastTenPlayers();
			bool allReady = true;
			for (int i = 0; i < players.Num(); i++) {
				if (players[i] == nullptr) {
					Helpers::PrintString("AHalfPastTenLogic::Tick() - Player is nullptr at " + FString::FromInt(i));
					continue;
				}
				if (!players[i]->bReady) {
					allReady = false;
					break;
				}
			}
			if (allReady) {
				//Print all readied players for sanity check
				for (int i = 0; i < players.Num(); i++) {
					if (players[i] == nullptr) {
						Helpers::PrintString("AHalfPastTenLogic::Tick() - all ready print - Player is nullptr at " + FString::FromInt(i));
						continue;
					}
					Helpers::PrintString("Player " + FString::FromInt(i) + " is ready");
				}

				Helpers::PrintString("All players are ready");

				//Deal first card to players
				this->DealCardToPlayers(players);	

				CurrentState = EHalfPastTenGameState::GS_DrawCard;
			}
		}

			break;
		case EHalfPastTenGameState::GS_DrawCard:
			//UE_LOG(LogTemp, Warning, TEXT("Draw Card"));
			break;
		case EHalfPastTenGameState::GS_CardDeal:
			//UE_LOG(LogTemp, Warning, TEXT("Deal Card To Highest Bidder"));
			break;
		case EHalfPastTenGameState::GS_End:
			//UE_LOG(LogTemp, Warning, TEXT("End Game"));
			break;
		default:
			//(LogTemp, Warning, TEXT("Invalid State"));
			break;
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Client"));
	}

}

void AHalfPastTenLogic::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHalfPastTenLogic, CurrentState);
}

