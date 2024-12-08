// Fill out your copyright notice in the Description page of Project Settings.


#include "SeatManager.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Helpers.h"

// Sets default values
ASeatManager::ASeatManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	bNetLoadOnClient = true;

	UE_LOG(LogTemp, Warning, TEXT("ASeatManager::ASeatManager - Replicates: %d"), bReplicates);

}

// Called when the game starts or when spawned
void ASeatManager::BeginPlay()
{
	Super::BeginPlay();
	
}
void ASeatManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 获取所有 APlayerBase 类型的玩家
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerBase::StaticClass(), FoundActors);

	//Helpers::PrintString(FString::Printf(TEXT("Found %d players"), FoundActors.Num()));

	// 将 FoundActors 转换成 APlayerBase* 数组，方便排序
	TArray<APlayerBase*> PlayerBases;
	for (AActor* Actor : FoundActors) {
		APlayerBase* Player = Cast<APlayerBase>(Actor);
		if (Player) {
			PlayerBases.Add(Player);
		}
	}

	// 按照 PlayerId 进行从小到大的排序
	PlayerBases.Sort([](const APlayerBase& LHS, const APlayerBase& RHS) {
		return LHS.PlayerId < RHS.PlayerId;
		});


	// 为每个玩家分配位置和朝向
	// 使用世界前方向为基准，然后根据玩家序号均匀环绕分布
	float AngleStep = (PlayerBases.Num() > 0) ? (360.0f / PlayerBases.Num()) : 0.0f;
	for (int i = 0; i < PlayerBases.Num(); i++) {
		APlayerBase* Player = PlayerBases[i];
		if (Player) {
			// 基于 ForwardVector 沿Z轴旋转，以实现圆周分布
			float CurrentAngle = AngleStep * i;
			FVector PlayerLocation;
			PlayerLocation.X = DeskRadius * FMath::Cos(FMath::DegreesToRadians(CurrentAngle));
			PlayerLocation.Y = DeskRadius * FMath::Sin(FMath::DegreesToRadians(CurrentAngle));
			PlayerLocation.Z = Player->SpawnHeight;

			FRotator NewRotation = UKismetMathLibrary::FindLookAtRotation(PlayerLocation, targetLocation);

			//Helpers::PrintString(FString::Printf(TEXT("PlayerId: %d, NewLocation: %s"), Player->PlayerId, *PlayerLocation.ToString()));

			Player->SetActorLocation(PlayerLocation);
			Player->SetActorRotation(NewRotation);


		}
	}

	Players = PlayerBases;
}




void ASeatManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Replicate the PlayerIdSequence array.
	//DOREPLIFETIME(ASeatManager, PlayerIdSequence);
}