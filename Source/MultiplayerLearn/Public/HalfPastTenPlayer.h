// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBase.h"
#include "HalfPastTenHandDeck.h"
#include "HalfPastTenPlayer.generated.h"

class AHandCard;

UCLASS()
class MULTIPLAYERLEARN_API AHalfPastTenPlayer : public APlayerBase
{
	GENERATED_BODY()

private:

	
public:
	// Tick
	virtual void Tick(float DeltaTime) override;

	//An array of the player's cards
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Player")
	TArray<int> PlayerCardValues;

	UFUNCTION(BlueprintCallable, Category = "Player")
	double GetTotalCardValues();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	bool bIsReady; //click ready after seeing the first card

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	AHalfPastTenHandDeck* HandDeck;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	double HandDeckHeight = 100.0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	double HandDeckDistance = 50.0;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
