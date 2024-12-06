// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameState.h"
#include "GameLogic.generated.h"


UCLASS()
class MULTIPLAYERLEARN_API AGameLogic : public AGameState
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGameLogic();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
