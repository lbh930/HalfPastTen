// Fill out your copyright notice in the Description page of Project Settings.


#include "HalfPastTenLogic.h"

// Sets default values
AHalfPastTenLogic::AHalfPastTenLogic()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CurrentState = EHalfPastTenGameState::GE_Start;
}

void AHalfPastTenLogic::BeginPlay()
{
	Super::BeginPlay();

}

void AHalfPastTenLogic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority()) {
		switch (CurrentState)
		{
		case EHalfPastTenGameState::GE_Start:
			//UE_LOG(LogTemp, Warning, TEXT("Start Game"));
			break;
		case EHalfPastTenGameState::GS_Initial:
			//UE_LOG(LogTemp, Warning, TEXT("Initial (Every one get a card)"));
			break;
		case EHalfPastTenGameState::GS_DrawCard:
			//UE_LOG(LogTemp, Warning, TEXT("Draw Card"));
			break;
		case EHalfPastTenGameState::GS_CardDeal:
			//UE_LOG(LogTemp, Warning, TEXT("Deal Card To Highest Bidder"));
			break;
		case EHalfPastTenGameState::GS_End:
			//UE_LOG(LogTemp, Warning, TEXT("End Game"));
			break;
		default:
			//(LogTemp, Warning, TEXT("Invalid State"));
			break;
		}
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Client"));
	}

}

