// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Card")
	void SetCardValue(int Value) { CardValue = Value; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	FString CardText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	int CardValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	UTextRenderComponent* TextComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Card")
	AHalfPastTenPlayer* OwningPlayer;

};
