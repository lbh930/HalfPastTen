// Fill out your copyright notice in the Description page of Project Settings.

// A CardPool is a visualizer of a set of deck without the need of a player to own it
// It is a deck of cards that is shown to all players
// ACardPool is NOT responsible for storing what cards remain in the deck - this logic should be handled elsewhere
// ACardPool IS responsible for replication of the card values it is showing

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SeatManager.h"
#include "HalfPastTenHandDeck.h"

#include "Net/UnrealNetwork.h"
#include "CardPool.generated.h"


//Reponsible for the replication of the Card Pool card Values
UCLASS()
class MULTIPLAYERLEARN_API ACardPool : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACardPool();
    virtual ~ACardPool();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	ASeatManager* SeatManager;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//An array of the card values that are currently being shown
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "CardPool")
	TArray<int> ShowingCardValues;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardPool", meta = (AllowPrivateAccess = "true"))
    AHalfPastTenHandDeck* HandDeck; //for visualizing the shown cards
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CardPool")
    FVector DeckPos = FVector(0, 0, 0);
    
    //For Replication
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    UFUNCTION(BlueprintCallable, Category = "CardPool")
    void AddCard(int CardValue) { ShowingCardValues.Add(CardValue); }
    
    UFUNCTION(BlueprintCallable, Category = "CardPool")
    void ClearCards() { ShowingCardValues.Empty(); }
    
    UFUNCTION(BlueprintCallable, Category = "CardPool")
    int GetFirstCard() { return ShowingCardValues.Num() > 0 ? ShowingCardValues[0] : -1; }
};
