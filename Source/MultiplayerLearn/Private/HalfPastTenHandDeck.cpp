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

void AHalfPastTenHandDeck::SetCardValues(TArray<int32>& CardValues)
{
	if (this->mCardValues != CardValues) {
		this->mCardValues = CardValues;
		GenCardActors();
	}
	else {
		this->mCardValues = CardValues;
	}
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
                CardActor->AttachToComponent(DeckBasePosition, FAttachmentTransformRules::KeepRelativeTransform);
                CardActor->SetActorRelativeLocation(FVector((double(i) - (double(n) / 2.0)) * CardGap, 0, 0));
                CardActors.Add(CardActor);
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


