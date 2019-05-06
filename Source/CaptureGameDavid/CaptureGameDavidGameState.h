// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CaptureGameTypes.h"
#include "CaptureGameDavidGameState.generated.h"


class ACaptureGameDavidCharacter;

USTRUCT()
struct FCaptureTeam
{
	GENERATED_BODY()

		TArray<ACaptureGameDavidCharacter*> mPlayers;
};

/**
 *
 */
UCLASS()
class CAPTUREGAMEDAVID_API ACaptureGameDavidGameState : public AGameStateBase
{
	GENERATED_UCLASS_BODY()

	void AddToTeam(ACaptureGameDavidCharacter* Character);
	void RemoveFromTeam(ACaptureGameDavidCharacter* Character);

	int32 GetNextTeamIndex() const;

	UPROPERTY(Transient, Replicated)
		TArray<FCaptureTeam> mTeams;



	UPROPERTY(Replicated)
	int BlueScore;

	UPROPERTY(Replicated)
	int RedScore;

};
