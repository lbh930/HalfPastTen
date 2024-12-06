// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameLogic.h"
#include "HalfPastTenLogic.generated.h"

UENUM(BlueprintType)
enum class EHalfPastTenGameState : uint8
{
	GE_Start UMETA(DisplayName = "Start Game"),
	GS_Initial UMETA(DisplayName = "Initial (Every one get a card)"),
	GS_DrawCard UMETA(DisplayName = "Draw Card"),
	GS_CardDeal UMETA(DisplayName = "Deal Card To Highest Bidder"),
	GS_End UMETA(DisplayName = "End Game")
};

/**
 * 
 */
UCLASS()
class MULTIPLAYERLEARN_API AHalfPastTenLogic : public AGameLogic
{
	GENERATED_BODY()

public:
	AHalfPastTenLogic();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameState")
	EHalfPastTenGameState CurrentState;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	
};
