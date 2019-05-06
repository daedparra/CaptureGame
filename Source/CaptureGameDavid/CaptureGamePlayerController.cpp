// Fill out your copyright notice in the Description page of Project Settings.

#include "CaptureGamePlayerController.h"


#include "ScoreWidget.h"

void ACaptureGamePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (GameInfoBP != nullptr && IsLocalController())
	{
		GameInfoWidget = CreateWidget<UScoreWidget>(this, GameInfoBP);
		GameInfoWidget->AddToViewport();
	}
}
