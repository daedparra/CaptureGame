// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CaptureGameDavidGameMode.generated.h"

UCLASS(minimalapi)
class ACaptureGameDavidGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ACaptureGameDavidGameMode();
	void RespawnPlayer(AController* Player, AActor* Spot);
	virtual void BeginPlay() override;

	UFUNCTION(Reliable, NetMulticast)
	void AddRedScore();
	UFUNCTION(Reliable, NetMulticast)
	void AddBlueScore();

	UFUNCTION()
	void SpawnPlayers();

	FTimerHandle	TimerHandle_Fuse;
	float FuseTime;

	TArray<AActor*> PlayerStartArray;

};



