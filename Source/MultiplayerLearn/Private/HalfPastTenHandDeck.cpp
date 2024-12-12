// Fill out your copyright notice in the Description page of Project Settings.


#include "HalfPastTenHandDeck.h"
#include "Helpers.h"

// Sets default values
AHalfPastTenHandDeck::AHalfPastTenHandDeck()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AHalfPastTenHandDeck::BeginPlay()
{
	Super::BeginPlay();
    GenCardActors();
	
}

// Called every frame
void AHalfPastTenHandDeck::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHalfPastTenHandDeck::SetCardValues(TArray<int32>& CardValues, TArray<bool> & bIsFaceUp, bool _bIsLocalPlayerDeck)
{
	if (this->mCardValues != CardValues) {
		this->mCardValues = CardValues;
		GenCardActors();
	}
	else {
		this->mCardValues = CardValues;
	}

	this->bIsLocalPlayerDeck = _bIsLocalPlayerDeck;
}
void AHalfPastTenHandDeck::GenCardActors(){
    if (CardActors.Num() > 0) {
        for (AHandCard* CardActor : CardActors) {
            if (CardActor) {
                CardActor->Destroy();
            }
        }
        CardActors.Empty();
    }

    int n = mCardValues.Num();

    for (int i = 0; i < n; i++) {
        if (HandCardClass) {
            AHandCard* CardActor = GetWorld()->SpawnActor<AHandCard>(HandCardClass, GetActorLocation(), GetActorRotation());
            if (CardActor) {
                CardActor->SetCardValue(mCardValues[i]);
				CardActor->SetIsFaceUp(mCardFaceUp[i]);
				Helpers::PrintString("Set Card Value: " + FString::FromInt(mCardValues[i]) + " FaceUp: " + (mCardFaceUp[i] ? "True" : "False"));
                CardActor->AttachToComponent(DeckBasePosition, FAttachmentTransformRules::KeepRelativeTransform);
                CardActor->SetActorRelativeLocation(FVector((double(i) - (double(n) / 2.0)) * CardGap, 0, 0));
				if (CardActor->bIsFaceUp) {
                    CardActor->SetActorRelativeRotation(FRotator(0, 0, 0));
                }
                else {
					CardActor->SetActorRelativeRotation(FRotator(0, 180, 0));
                }
				
                CardActors.Add(CardActor);

				CardActor->SetIsLocalPlayerCard(bIsLocalPlayerDeck);
            }
            else {
				Helpers::PrintString("CardActor is null!");
            }
        }
        else {
            Helpers::PrintString("HandCardClass is null!");
        }
    }
}


