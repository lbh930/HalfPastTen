// Fill out your copyright notice in the Description page of Project Settings.


#include "CardPool.h"
#include "Helpers.h"

// Sets default values
ACardPool::ACardPool()
{
    // Set this actor to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Enable replication
    bReplicates = true;  
}




ACardPool::~ACardPool() {
    // Implementation (even if empty)
}

// Called when the game starts or when spawned
void ACardPool::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACardPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Show the cards
    if (HandDeck) {
        HandDeck->SetActorLocation(DeckPos);
        TArray<bool> FaceUp;
        FaceUp.Init(true, ShowingCardValues.Num());
        HandDeck->SetCardValues(ShowingCardValues, FaceUp, false);
    }else{
        Helpers::PrintString("ACardPool::Tick() - HandDeck is nullptr");
    }
}

void ACardPool::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACardPool, ShowingCardValues);
}
