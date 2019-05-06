// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "CaptureGameDavidGameMode.h"
#include "CaptureGameDavidCharacter.h"
#include "CaptureGameDavidGameState.h"
#include "CaptureGameDavidGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "Runtime/Engine/Public/TimerManager.h"

ACaptureGameDavidGameMode::ACaptureGameDavidGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	static ConstructorHelpers::FClassFinder<AGameStateBase> GameStateBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/CaptureGameState"));
	if (GameStateBPClass.Class != NULL)
	{
		GameStateClass = GameStateBPClass.Class;
	}
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/BP_PlayerController"));
	if (PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}
	FuseTime = 1.f;
}


void ACaptureGameDavidGameMode::RespawnPlayer(AController* Player, AActor* Spot)
{
	ACaptureGameDavidCharacter* Pawn = Cast<ACaptureGameDavidCharacter>(Player->GetPawn());

	Pawn->SetActorLocation(Spot->GetActorLocation());

	//Pawn->SetActorTransform(Spot->GetActorTransform());
	Pawn->SetActorRotation(Spot->GetActorRotation());

	Player->SetControlRotation(Spot->GetActorRotation());
}

void ACaptureGameDavidGameMode::AddRedScore_Implementation()
{
	if (Role == ROLE_Authority)
	{
		UWorld* World = GetWorld();

		if (World != nullptr)
		{
			ACaptureGameDavidGameState* GRI = World->GetGameState<ACaptureGameDavidGameState>();

			if (GRI != nullptr)
			{
				GRI->RedScore++;
			}
		}
	}
}

void ACaptureGameDavidGameMode::AddBlueScore_Implementation()
{
	
	if (Role == ROLE_Authority)
	{
		UWorld* World = GetWorld();

		if (World != nullptr)
		{
			ACaptureGameDavidGameState* GRI = World->GetGameState<ACaptureGameDavidGameState>();

			if (GRI != nullptr)
			{
				GRI->BlueScore++;
			}
		}
	}
}

#pragma optimize( "", off )

void ACaptureGameDavidGameMode::BeginPlay()
{
	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		World->GetTimerManager().SetTimer(TimerHandle_Fuse, this, &ACaptureGameDavidGameMode::SpawnPlayers, FuseTime);
	}
	

			/*for (int32 i = 1; i < (int32)ECaptureTeam::MAX; ++i)
			{
				const int32 NumPlayers = GRI->mTeams[i].mPlayers.Num();

				UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStartArray);
				for (int32 y = 0; y < PlayerStartArray.Num(); y++)
				{
					APlayerStart* Player = Cast<APlayerStart>(PlayerStartArray[y]);
					int32 tag = FCString::Atoi(*Player->PlayerStartTag.ToString());
					if (tag == 0 && GRI->mTeams[i].mPlayers[0]->GetTeam() == ECaptureTeam::Red || tag == 1 && GRI->mTeams[i].mPlayers[0]->GetTeam() == ECaptureTeam::Red)
					{
						RespawnPlayer(GRI->mTeams[i].mPlayers[0]->GetController(), Player);
						break;
					}
					if (tag == 2 && GRI->mTeams[i].mPlayers[0]->GetTeam() == ECaptureTeam::Blue || tag == 3 && GRI->mTeams[i].mPlayers[0]->GetTeam() == ECaptureTeam::Blue)
					{
						RespawnPlayer(GRI->mTeams[i].mPlayers[0]->GetController(), Player);
						break;
					}
				}
			}*/
}

void ACaptureGameDavidGameMode::SpawnPlayers()
{
	UWorld* World = GetWorld();

	if (World != nullptr)
	{
		ACaptureGameDavidGameState* GRI = World->GetGameState<ACaptureGameDavidGameState>();
		if (GRI != nullptr)
		{
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStartArray);

			TArray<AActor*> PlayerStartArray2;
			PlayerStartArray2.AddDefaulted((int32)ECaptureTeam::MAX * 2);

			for (AActor* PlayerStart : PlayerStartArray)
			{
				APlayerStart* PS = Cast<APlayerStart>(PlayerStart);

				int32 tag = FCString::Atoi(*PS->PlayerStartTag.ToString());

				if (PS != nullptr)
				{
					PlayerStartArray2[tag] = PlayerStart;
				}
			}

			for (int32 i = 0; i < (int32)ECaptureTeam::MAX; ++i)
			{
				FCaptureTeam& captureTeam = GRI->mTeams[i];

				for (int32 y = 0; y < captureTeam.mPlayers.Num(); ++y)
				{
					int32 tagIndex = (i * 2) + y;

					if (PlayerStartArray2.IsValidIndex(tagIndex))
					{
						RespawnPlayer(captureTeam.mPlayers[y]->GetController(), PlayerStartArray2[tagIndex]);
					}
				}
			}
		}
	}
}

#pragma optimize( "", on )