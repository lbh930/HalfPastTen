// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"

#include "Components/TextRenderComponent.h"

#include "HandCard.generated.h"

class AHalfPastTenPlayer;

UCLASS()
class MULTIPLAYERLEARN_API AHandCard : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHandCard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
    
    //collision component
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
    UBoxComponent* CollisionComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Card")
	void SetCardValue(int Value) { CardValue = Value; }

	UFUNCTION(BluePrintCallable, Category = "Card")
	void SetIsFaceUp(bool _bIsFaceUp) { bIsFaceUp = _bIsFaceUp; }

	UFUNCTION(BlueprintCallable, Category = "Card")
	void SetIsLocalPlayerCard(bool _bIsLocalPlayerCard);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	FString CardText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	int CardValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	UTextRenderComponent* TextComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	AHalfPastTenPlayer* OwningPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	bool bIsFaceUp = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	bool bIsLocalPlayerCard = false;
    
    UFUNCTION(BlueprintCallable, Category = "Card")
    void OnMouseHoverBegin(UPrimitiveComponent* TouchedComponent);

    UFUNCTION(BlueprintCallable, Category = "Card")
    void OnMouseHoverEnd(UPrimitiveComponent* TouchedComponent);

};
