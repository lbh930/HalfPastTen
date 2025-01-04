// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameLogic.h"
#include "CardPool.h"
#include "SeatManager.h"
#include "HandCard.h"
#include "HalfPastTenLogic.generated.h"

UENUM(BlueprintType)
enum class EHalfPastTenGameState : uint8
{
	GE_Start UMETA(DisplayName = "Start Game"),
	GS_Initial UMETA(DisplayName = "Initial (Every one get a card)"),
	GS_DrawCard UMETA(DisplayName = "Draw Card"),
	GS_CardDeal UMETA(DisplayName = "Deal Card To Highest Bidder"),
	GS_End UMETA(DisplayName = "End Game")
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MULTIPLAYERLEARN_API AHalfPastTenLogic : public AGameLogic
{
	GENERATED_BODY()

public:
	AHalfPastTenLogic();

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Game_Auto")
	EHalfPastTenGameState CurrentState;
    UFUNCTION(BlueprintCallable)
    EHalfPastTenGameState GetCurrentState() { return CurrentState; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game_Manual")
	AActor* DrawDeckPos;
    
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Game_Auto")
    int HighestBid;
    UFUNCTION(BlueprintCallable)
    int GetHighestBid() { return HighestBid; }
    
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Game_Auto")
    int HighestBidPlayerId = -1;
    UFUNCTION(BlueprintCallable)
    int GetHighestBidPlayerId() { return HighestBidPlayerId; }
    
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Game_Manual")
    int MaximumBid = 200;
    UFUNCTION(BlueprintCallable)
    int GetMaximumBid() { return MaximumBid; }
    
    
    UFUNCTION(BlueprintCallable, Category = "Game_Auto")
    void TryBid(int playerId, int bid);

	UFUNCTION(BlueprintCallable, Category = "Game_Auto")
	void TryStrike(int playerId, int bid);

	//replication map func
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//debug related
	UFUNCTION(BlueprintCallable, Category = "Game")
	FString GetStatusString();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game_Auto_Ref")
	ASeatManager* SeatManager;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game_Auto_Ref")
    ACardPool* CardPool;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game_Auto_Ref")
    AHalfPastTenHandDeck* CardDeck; //for visualizing the shown cards

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game_Auto")
	TArray<int> RemainingDeck;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game_Auto")
	bool bFirstCardDealt = false;

	UFUNCTION(BlueprintCallable, Category = "Game_Auto")
	void DealCardToPlayers(const TArray<AHalfPastTenPlayer*> & players, bool bIsFaceUp);

	UFUNCTION(BlueprintCallable, Category = "Game_Auto")
	int DrawCardFromDeck();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game_Auto")
	bool bCardDrawn = false;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game_Auto")
	AHandCard* DrawnCard;

public: //Getter and Setters
	UFUNCTION(BlueprintCallable)
	void SetPassCount(int count) { PassCount = count; }
	UFUNCTION(BlueprintCallable)
	int GetPassCount() { return PassCount; }

	UFUNCTION(BlueprintCallable)
	void SetStrikeCount(int count) { StrikeCount = count; }
	UFUNCTION(BlueprintCallable)
	int GetStrikeCount() { return StrikeCount; }

	UFUNCTION(BlueprintCallable)
	void SetIsStrike(bool bStrike) { bIsStrike = bStrike; }
	UFUNCTION(BlueprintCallable)
	bool GetIsStrike() { return bIsStrike; }

	UFUNCTION(BlueprintCallable)
	void GetMaximumPassCount(int count) { MaximumPassCount = count; }
	
	UFUNCTION(BlueprintCallable)
	void GetMaximumStrikeCount(int count) { MaximumStrikeCount = count; }

protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Game_Auto")
	int PassCount;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Game_Auto")
	int StrikeCount;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Game_Auto")
	bool bIsStrike = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game_Auto")
	int MaximumPassCount = 3;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game_Auto")
	int MaximumStrikeCount = 3;

	
    
private:
    FTimerHandle stateChangeTimerHandle;

    void ChangeStateTimed(EHalfPastTenGameState newState, float delayTime);
    
    void ChangeState(EHalfPastTenGameState newState);

    bool bStateChangeTimerRunning = false;
	
};
