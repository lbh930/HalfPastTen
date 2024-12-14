// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerBase.h"
#include "HalfPastTenPlayer.h"
#include "Helpers.h"
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
    
	UFUNCTION(BlueprintCallable, Category = "Seats")
    TArray<APlayerBase*> GetPlayers() {
        for (int i = 0; i < Players.Num(); i++){
            if (Players[i] == nullptr) {
                Helpers::PrintString("ASeatManager::GetPlayers() 1 - Players[] is nullptr at " + FString::FromInt(i));
                continue;
            }
        }
        return Players;
    }

	UFUNCTION(BlueprintCallable, Category = "Seats")
	TArray<AHalfPastTenPlayer*> GetHalfPastTenPlayers() {
		TArray<AHalfPastTenPlayer*> halfPastTenPlayers;
		for (APlayerBase* player : Players) {
			if (AHalfPastTenPlayer* halfPastTenPlayer = Cast<AHalfPastTenPlayer>(player)) {
				halfPastTenPlayers.Add(halfPastTenPlayer);
            }else{
                Helpers::PrintString("ASeatManager::GetHalfPastTenPlayers() - Player is not AHalfPastTenPlayer");
            }
		}
		return halfPastTenPlayers;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seats")
	double DeskRadius = 100.0f;

	//transform of the target to look at
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Seats")
	FVector targetLocation;
};
