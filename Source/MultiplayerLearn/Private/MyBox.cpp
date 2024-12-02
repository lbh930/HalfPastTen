// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBox.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"


// Sets default values
AMyBox::AMyBox()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ReplicatedVar = 100;

}

// Called when the game starts or when spawned
void AMyBox::BeginPlay()
{
	Super::BeginPlay();

	SetReplicates(true);
	SetReplicateMovement(true);

	if (HasAuthority()) {
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AMyBox::MulticastRPCFunction, 4.0f, true, 2.0f);
	}

}

// Called every frame
void AMyBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMyBox::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AMyBox, ReplicatedVar);
}

void AMyBox::OnRep_ReplicatedVar()
{
	//UE_LOG(LogTemp, Warning, TEXT("ReplicatedVar has been changed"));
	if (HasAuthority())
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Server: OnRep_ReplicatedVar"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Client %d: OnRep_ReplicatedVar"), UE::GetPlayInEditorID()));
	}
}

void AMyBox::MulticastRPCFunction_Implementation()
{
	if (HasAuthority()) {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Server: MulticastRPCFunction_Implementation"));
	}
	else {
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Client %d: MulticastRPCFunction_Implementation"), UE::GetPlayInEditorID()));
	}

	if (!IsRunningDedicatedServer()) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorLocation() + FVector(0,0,100.0f),
			FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);

		
	}
}