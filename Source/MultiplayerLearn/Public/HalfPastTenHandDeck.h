// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HandCard.h"
#include "HalfPastTenHandDeck.generated.h"

UCLASS(Blueprintable)
class MULTIPLAYERLEARN_API AHalfPastTenHandDeck : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHalfPastTenHandDeck();

	void SetCardValues(TArray<int32>& CardValues, TArray<bool>& bIsFaceUp, bool _bIsLocalPlayerDeck);
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GenCardActors();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Deck")
	USceneComponent* DeckBasePosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck")
	double CardGap; //Gap between cards

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck")
	TArray<AHandCard*> CardActors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck")
	TArray<int32> mCardValues;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck")
	TArray<bool> mCardFaceUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck")
	TSubclassOf<AHandCard> HandCardClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Deck")
	bool bIsLocalPlayerDeck = false;

	
};
