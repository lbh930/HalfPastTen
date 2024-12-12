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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game_Manual")
	AActor* DrawDeckPos;

	//replication map func
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

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

	
};
