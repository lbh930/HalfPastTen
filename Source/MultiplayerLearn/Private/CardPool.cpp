// Fill out your copyright notice in the Description page of Project Settings.


#include "CardPool.h"

// Sets default values
ACardPool::ACardPool()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACardPool::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACardPool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Show the cards


}

