// Fill out your copyright notice in the Description page of Project Settings.


#include "HandCard.h"
#include "HalfPastTenPlayer.h"

// Sets default values
AHandCard::AHandCard()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CardValue = 1;
	CardText = "A";

}

// Called when the game starts or when spawned
void AHandCard::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHandCard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (TextComponent) {
		switch (CardValue) {
		case 1:
			TextComponent->SetText(FText::FromString("A"));
			break;
		case 11:
			TextComponent->SetText(FText::FromString("J"));
			break;
		case 12:
			TextComponent->SetText(FText::FromString("Q"));
			break;
		case 13:
			TextComponent->SetText(FText::FromString("K"));
			break;
		default:
			TextComponent->SetText(FText::FromString(FString::FromInt(CardValue)));
			break;
		}
	}


}
