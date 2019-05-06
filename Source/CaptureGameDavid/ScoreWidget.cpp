// Fill out your copyright notice in the Description page of Project Settings.

#include "ScoreWidget.h"

#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "CaptureGameDavidCharacter.h"
#include "CaptureGameDavidGameState.h"
#include "UnrealNetwork.h"
bool UScoreWidget::Initialize()
{
	bool bResult = Super::Initialize();
	if (!bResult)
	{
		return false;
	}

	return true;
}
//static int myValue = 0;

void UScoreWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	NetMulticastNum();
}


//This function will call it via server so it can change the value of grenades in the player...
void UScoreWidget::NetMulticastNum_Implementation() {
		UWorld* World = GetWorld();

		if (World != nullptr)
		{
			ACaptureGameDavidGameState* GRI = World->GetGameState<ACaptureGameDavidGameState>();

			if (GRI != nullptr)
			{
				FString RedValue = FString::Printf(TEXT("Red Score: %d"), GRI->RedScore);
				UTextBlock * RedWidget = Cast<UTextBlock>(GetWidgetFromName("Red"));
				if (RedWidget != nullptr)
				{
					RedWidget->SetText(FText::FromString(RedValue));
				}

				FString BlueValue = FString::Printf(TEXT("Blue Score: %d"), GRI->BlueScore);
				UTextBlock * BlueWidget = Cast<UTextBlock>(GetWidgetFromName("Blue"));
				if (BlueWidget != nullptr)
				{
					BlueWidget->SetText(FText::FromString(BlueValue));
				}
			}
		}
}
