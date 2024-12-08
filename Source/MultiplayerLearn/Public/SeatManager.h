// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerBase.h"
#include "HalfPastTenPlayer.h"
#include "SeatManager.generated.h"


UCLASS()
class MULTIPLAYERLEARN_API ASeatManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASeatManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	TArray<APlayerBase*> Players;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Seats")
	TArray<APlayerBase*> GetPlayers() { return Players; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seats")
	double DeskRadius = 100.0f;

	//transform of the target to look at
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seats")
	FVector targetLocation;
};
