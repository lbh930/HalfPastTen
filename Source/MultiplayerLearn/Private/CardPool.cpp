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

    // Check if the world is valid before spawning
    UWorld* World = GetWorld();
    if (World)
    {
        // Spawn the card deck
        HandDeck = World->SpawnActor<AHalfPastTenHandDeck>(AHalfPastTenHandDeck::StaticClass());
        if (!HandDeck)
        {
            Helpers::PrintString("ACardPool::ACardPool() - HandDeck is not Spawned Properly");
        }
        else
        {
            HandDeck->SetOwner(this);
        }
    }
    else
    {
        Helpers::PrintString("ACardPool::ACardPool() - GetWorld() returned nullptr");
    }
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
}

void ACardPool::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACardPool, ShowingCardValues);
}
