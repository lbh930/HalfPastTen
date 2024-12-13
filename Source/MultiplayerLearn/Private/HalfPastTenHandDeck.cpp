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
        this->mCardFaceUp = bIsFaceUp;
		GenCardActors();
	}
	else {
		this->mCardValues = CardValues;
        this->mCardFaceUp = bIsFaceUp;
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
    
    if (mCardValues.Num() != mCardFaceUp.Num()) {
        Helpers::PrintString("HalfPastTenHandDeck::GenCardActors() - CardValues and FaceUp array size mismatch! " +
            FString::FromInt(mCardValues.Num()) + " vs " + FString::FromInt(mCardFaceUp.Num()) + " aborting function.");
        return;
    }

    for (int i = 0; i < n; i++) {
        if (HandCardClass) {
            AHandCard* CardActor = GetWorld()->SpawnActor<AHandCard>(HandCardClass, GetActorLocation(), GetActorRotation());
            if (CardActor) {
                CardActor->SetCardValue(mCardValues[i]);
				CardActor->SetIsFaceUp(mCardFaceUp[i]);
				Helpers::PrintString("Set Card Value: " + FString::FromInt(mCardValues[i]) + " FaceUp: " + (mCardFaceUp[i] ? "True" : "False"));
                CardActor->AttachToComponent(DeckBasePosition, FAttachmentTransformRules::KeepRelativeTransform);
                CardActor->SetActorRelativeLocation(FVector((double(i) - (double(n) / 2.0)) * CardGap, 0, 0));
                
				if (mCardFaceUp[i]) {
                    Helpers::PrintString("Card is face up, set rotator zero");
                    CardActor->SetActorRelativeRotation(FRotator(0, 0, 0));
                }
                else {
                    Helpers::PrintString("Card is face down, set rotator 180");
					CardActor->SetActorRelativeRotation(FRotator(180, 0, 0));
                }
                
                //print current location and rotation for sanity check
                Helpers::PrintString("CardActor Location: " + CardActor->GetActorLocation().ToString());
                Helpers::PrintString("CardActor Rotation: " + CardActor->GetActorRotation().ToString());
                Helpers::PrintString ("-------------------");
				
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


