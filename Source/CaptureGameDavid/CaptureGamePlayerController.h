// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CaptureGamePlayerController.generated.h"

/**
 * 
 */
class UScoreWidget;
UCLASS()
class CAPTUREGAMEDAVID_API ACaptureGamePlayerController : public APlayerController
{
	GENERATED_BODY()
		virtual void BeginPlay() override;
		UPROPERTY()
			UScoreWidget* GameInfoWidget;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UScoreWidget> GameInfoBP;
};
