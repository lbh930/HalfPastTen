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
    
    //Get the card pool in the scene
    this->CardPool = Cast<ACardPool>(UGameplayStatics::GetActorOfClass(GetWorld(), ACardPool::StaticClass()));
    if (!CardPool) {
        Helpers::PrintString("No CardPool found in the scene");
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
			break;
		}
	}
	return resCard;
}

void AHalfPastTenLogic::DealCardToPlayers(const TArray<AHalfPastTenPlayer*>& players, bool IsFaceUp) {
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
			players[playerIndex]->DealCard(card, IsFaceUp);
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
			if (!bFirstCardDealt) {
				this->DealCardToPlayers(players, false);
				bFirstCardDealt = true;
			}

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

				CurrentState = EHalfPastTenGameState::GS_DrawCard;
			}
		}

			break;
		case EHalfPastTenGameState::GS_DrawCard:
			//UE_LOG(LogTemp, Warning, TEXT("Draw Card"));
                
            if (!bCardDrawn) {
                int card = this->DrawCardFromDeck();
                if (card == -1) {
                    Helpers::PrintString("No card left in deck! Set to 1 by default");
                    card = 1;
                }else{
                    Helpers::PrintString("AHalfPastTenLogic::Tick() - Card drawn: " + FString::FromInt(card));
                }
                
                if (CardPool){
                    CardPool->ShowingCardValues.Add(card);
                }else{
                    Helpers::PrintString("AHalfPastTenLogic::Tick() - CardPool is NULL");
                }
                
                bCardDrawn = true;
            }else{
                //wait for the players to bid
            }
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
    
    DOREPLIFETIME(AHalfPastTenLogic, HighestBid);
    
    DOREPLIFETIME(AHalfPastTenLogic, HighestBidPlayerId);
}

void AHalfPastTenLogic::TryBid(int playerId, int bid)
{
    Helpers::PrintString("AHalfPastTenLogic::ServerTryBid_Implementation() - Player " + FString::FromInt(playerId) + " bids " + FString::FromInt(bid));
    if (HasAuthority()){
        if (bid > HighestBid){
            HighestBid = bid;
            HighestBidPlayerId = playerId;
            Helpers::PrintString("AHalfPastTenLogic::ServerTryBid_Implementation() - Player " + FString::FromInt(playerId) + " has the highest bid of " + FString::FromInt(bid) + "! Set");
        }
    }else{
        Helpers::PrintString("AHalfPastTenLogic::ServerTryBid_Implementation() - Not authority");
    }
}

