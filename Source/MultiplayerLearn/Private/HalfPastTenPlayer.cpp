// Fill out your copyright notice in the Description page of Project Settings.


#include "HalfPastTenPlayer.h"
#include "HandCard.h"
#include "Net/UnrealNetwork.h"
#include "Helpers.h"

double AHalfPastTenPlayer::GetTotalCardValues()
{
	double sum = 0;
	for (int i = 0; i < PlayerCardValues.Num(); i++) {
		if (PlayerCardValues[i] <= 10 && PlayerCardValues[i] >= 1) {
			sum += double(PlayerCardValues[i]);
		}
		else if (PlayerCardValues[i] > 10 && PlayerCardValues[i] <= 13) {
			sum += 0.5;
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("PlayerCards[%d] has an invalid value"), i);
		}
	}
	return sum;
}

void AHalfPastTenPlayer::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AHalfPastTenPlayer, PlayerCardValues);
}

void AHalfPastTenPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HandDeck) {
		HandDeck->SetActorLocation(FVector(0, 0, HandDeckHeight));
		FVector ActorForwardVector = GetActorForwardVector();
		ActorForwardVector.Z = 0;
		HandDeck->SetActorLocation(GetActorLocation() + ActorForwardVector * HandDeckDistance);
		HandDeck->SetActorRotation(GetActorRotation());
		HandDeck->SetCardValues(PlayerCardValues);
	}
	else {
		Helpers::PrintString("HandDeck is not set for player");
	}
}