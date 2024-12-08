// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerBase.generated.h"

UCLASS()
class MULTIPLAYERLEARN_API APlayerBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Player")
	int PlayerId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	double SpawnHeight = 100.0f;

	//replication map func
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable, Category = "Player")
	void GenPlayerId();

};
