// Fill out your copyright notice in the Description page of Project Settings.

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

	UPROPERTY(VisibleAnywhere)
	AHalfPastTenHandDeck* HandDeck;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//An array of the card values that are currently being shown
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "CardPool")
	TArray<int> ShowingCardValues;
    
    //For Replication
    void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
    
    


};
