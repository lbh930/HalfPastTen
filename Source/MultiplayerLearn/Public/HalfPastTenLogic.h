// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameLogic.h"
#include "SeatManager.h"
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

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Game")
	EHalfPastTenGameState CurrentState;

	//replication map func
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game")
	ASeatManager* SeatManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game")
	TArray<int> RemainingDeck;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game")
	bool bFirstCardDealt = false;

	UFUNCTION(BlueprintCallable, Category = "Game")
	void DealCardToPlayers(const TArray<AHalfPastTenPlayer*> & players);

	UFUNCTION(BlueprintCallable, Category = "Game")
	int DrawCardFromDeck();
	
};
