// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerBase.h"
#include "HalfPastTenPlayer.generated.h"

class AHandCard;

UCLASS()
class MULTIPLAYERLEARN_API AHalfPastTenPlayer : public APlayerBase
{
	GENERATED_BODY()

private:

	
public:

	//An array of the player's cards
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Player")
	TArray<int> PlayerCardValues;

	UFUNCTION(BlueprintCallable, Category = "Player")
	double GetTotalCardValues();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	bool bIsReady; //click ready after seeing the first card

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
