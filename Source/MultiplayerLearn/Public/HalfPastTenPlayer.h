// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBase.h"
#include "HalfPastTenHandDeck.h"

//include widget and text related headers
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
//where is getHud() from?
#include "GameFramework/PlayerController.h"

#include "HalfPastTenPlayer.generated.h"

class AHandCard;
class ASeatManager;
class AHalfPastTenLogic;

UCLASS()
class MULTIPLAYERLEARN_API AHalfPastTenPlayer : public APlayerBase
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	ASeatManager* SeatManager;
    
    UPROPERTY(VisibleAnywhere)
    AHalfPastTenLogic* HalfPastTenLogic;

	//beginplay
	virtual void BeginPlay() override;

public:
	AHalfPastTenPlayer();

	// Tick
	virtual void Tick(float DeltaTime) override;

	//An array of the player's cards
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Player")
	TArray<int> PlayerCardValues;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Player")
	TArray<bool> PlayerCardFaceUp;

	UFUNCTION(BlueprintCallable, Category = "Player")
	double GetTotalCardValues();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	AHalfPastTenHandDeck* HandDeck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	double HandDeckHeight = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	double HandDeckDistance = 50.0;

	//For replication of Readyness
	UFUNCTION(Server, BlueprintCallable, Reliable)
	void ServerSetReady(bool bNewReady);

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Player")
	bool bReady = false;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player")
    int CurrentBid = 0;
    UFUNCTION (BlueprintCallable, Category = "Player")
    void SetCurrentBid(int newBid);
    UFUNCTION (BlueprintCallable, Category = "Player")
    int GetCurrentBid();
    
    UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Player")
    bool bHasWaived = false;
    UFUNCTION (BlueprintCallable, Category = "Player")
    void SetHasWaived(bool bNewHasWaived){bHasWaived = bNewHasWaived;}
    UFUNCTION (BlueprintCallable, Category = "Player")
    bool GetHasWaived(){return bHasWaived;}
    
    UFUNCTION(Server, BlueprintCallable, Category = "Player", Reliable)
    void ServerBid();
    

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	//Text
	UFUNCTION(BlueprintCallable, Category = "Player")
	void UpdatePlayerWidgetText(UTextBlock* textBlock, const FString& newText);

	UFUNCTION(BlueprintCallable, Category = "Player")
	FString GetPlayerReadyText();

	UFUNCTION(BlueprintCallable, Category = "Player")
	void DealCard(int cardValue, bool bIsFaceUp);
    
    

};
