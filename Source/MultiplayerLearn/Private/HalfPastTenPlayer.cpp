// Fill out your copyright notice in the Description page of Project Settings.

#include "HalfPastTenPlayer.h"
#include "HandCard.h"
#include "Math/UnrealMathUtility.h"
#include "Net/UnrealNetwork.h"
#include "Helpers.h"
#include "SeatManager.h"
#include "HalfPastTenLogic.h"
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
    
    //get the game state in the scene
    HalfPastTenLogic = Cast<AHalfPastTenLogic>(GetWorld()->GetGameState());
    
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
    DOREPLIFETIME(AHalfPastTenPlayer, bHasWaived);
    
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

	//local player logics
	if (IsLocallyControlled()){
		if (HalfPastTenLogic){
			if (IsLocallyControlled() && HalfPastTenLogic->GetCurrentState() != LastState)
			{
				LastState = HalfPastTenLogic->GetCurrentState();
				OnStateChanged(LastState);
			}
			
			if (HalfPastTenLogic->GetCurrentState() == EHalfPastTenGameState::GS_DrawCard){
				if (HalfPastTenLogic->GetIsStrike()){
					CurrentBid = FMath::Max(CurrentBid, HalfPastTenLogic->GetHighestBid()*2);
				}else
				{
					CurrentBid = FMath::Max(CurrentBid, HalfPastTenLogic->GetHighestBid());
				}
            }
			
		}else{
			Helpers::PrintString("AHalfPastTenPlayer::Tick() - HalfPastTenLogic is not set for player");
		}
	}
}

FString AHalfPastTenPlayer::GetPlayerReadyText() {
	//Set Readyness text
	if (SeatManager) {
		FString readyText = "";
		TArray<AHalfPastTenPlayer*> PlayerBases = SeatManager->GetHalfPastTenPlayers();
        
        for (int i = 0; i < PlayerBases.Num(); i++){
            if (PlayerBases[i] == nullptr) {
                Helpers::PrintString("AHalfPastTenPlayer::GetPlayerReadyText() 1 - PlayerBases[] is nullptr at " + FString::FromInt(i));
                continue;
            }
        }

		//Helpers::PrintString("PlayerBases.Num(): " + FString::FromInt(PlayerBases.Num()));

		PlayerBases.Sort([](const APlayerBase& LHS, const APlayerBase& RHS) {
			return LHS.PlayerId < RHS.PlayerId;
			});

		for (int i = 0; i < PlayerBases.Num(); i++) {
			if (PlayerBases[i] == nullptr) {
				Helpers::PrintString("AHalfPastTenPlayer::GetPlayerReadyText() 2~ - PlayerBases[] is nullptr at " + FString::FromInt(i));
				continue;
			}
			AHalfPastTenPlayer* Player = PlayerBases[i];
			if (Player) {
				readyText += FString::Printf(TEXT("Player %d: %s\n"), Player->PlayerId, Player->bReady ? TEXT("Ready") : TEXT("Not Ready"));
			}
			else {
				Helpers::PrintString("AHalfPastTenPlayer::GetPlayerReadyText() 3?? - Players[] is nullptr at " + FString::FromInt(i));
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

void AHalfPastTenPlayer::SetCurrentBid(int newBid)
{
    CurrentBid = newBid;
}

int AHalfPastTenPlayer::GetCurrentBid(){
    return CurrentBid;
}

void AHalfPastTenPlayer::ServerBid_Implementation(int _playerId, int _currentBid){
    if (HasAuthority()){
        Helpers::PrintString("AHalfPastTenPlayer::Bid() - Player " + FString::FromInt(PlayerId) + " bids " + FString::FromInt(CurrentBid));
        if (HalfPastTenLogic){
            HalfPastTenLogic->TryBid(_playerId, _currentBid);
        }else{
            Helpers::PrintString("AHalfPastTenPlayer::Bid() - HalfPastTenLogic is not set for player");
        }
    }
}

void AHalfPastTenPlayer::ServerWaive_Implementation(){
    if (HasAuthority()){
    	/* Player can only waive when they either
    	1. Have not bid in this round
    	2. Have bid but is not the highest bidder
    	*/
    	if (HalfPastTenLogic){
    		if (HalfPastTenLogic->GetHighestBid() == 0 ||
    			HalfPastTenLogic->GetHighestBidPlayerId() != PlayerId)
    		{
    			bHasWaived = true;
    		}
    	}else
    	{
    		Helpers::PrintString("AHalfPastTenPlayer::ServerWaive_Implementation() - HalfPastTenLogic is not set for player");
    	}
    }else
    {
	    Helpers::PrintString("AHalfPastTenPlayer::ServerWaive_Implementation() - Not authority");
    }
}

void AHalfPastTenPlayer::ServerStrike_Implementation(int _playerId, int _currentBid)
{
	if (HasAuthority()){
		if (HalfPastTenLogic){
			HalfPastTenLogic->TryStrike(_playerId, _currentBid);
		}else{
			Helpers::PrintString("AHalfPastTenPlayer::ServerStrike_Implementation() - HalfPastTenLogic is not set for player");
		}
	}else{
		Helpers::PrintString("AHalfPastTenPlayer::ServerStrike_Implementation() - Not authority");
	}
}

void AHalfPastTenPlayer::OnStateChanged(EHalfPastTenGameState newState)
{
	if (!IsLocallyControlled()) return;
	
	if (newState == EHalfPastTenGameState::GS_CardDeal) {
		CurrentBid = 0;
	}else if (newState == EHalfPastTenGameState::GS_DrawCard){
		CurrentBid = FMath::Max(CurrentBid, HalfPastTenLogic->GetHighestBid());
	}
}
