// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "SeatManager.h"
#include "Helpers.h"

// Sets default values
APlayerBase::APlayerBase()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//replicate position
	bReplicates = true;
	//bNetLoadOnClient = true;
}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) {
		GenPlayerId();

	}

}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayerBase, PlayerId);
}

void APlayerBase::GenPlayerId()
{
	PlayerId = FMath::RandRange(0, 100000);
	Helpers::PrintString(FString::Printf(TEXT("PlayerId set to: %d"), PlayerId));
}

