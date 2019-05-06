// Fill out your copyright notice in the Description page of Project Settings.

#include "CaptureGameBaseScore.h"
#include "Components/BoxComponent.h"
#include "Components/TextRenderComponent.h"
#include "CaptureGameDavidCharacter.h"
#include "CaptureGameDavidFlag.h"
#include "CaptureGameDavidGameMode.h"
#include "CaptureGameTypes.h"
// Sets default values
ACaptureGameBaseScore::ACaptureGameBaseScore()
{
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	TextComponent = CreateDefaultSubobject<UTextRenderComponent>(TEXT("TextComponent"));
	TextComponent->AttachTo(RootComponent);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACaptureGameBaseScore::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ACaptureGameBaseScore::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ACaptureGameBaseScore::OnOverlapEnd);
}

void ACaptureGameBaseScore::OnOverlapBegin(AActor * OverlappedActor, AActor * OtherActor)
{
	ACaptureGameDavidCharacter* player = Cast<ACaptureGameDavidCharacter>(OtherActor);
	if (player != nullptr)
	{
		if (player->Myflag != nullptr) {
			UWorld* World = GetWorld();
			if (World != nullptr)
			{
				ACaptureGameDavidGameMode* GRI = World->GetAuthGameMode<ACaptureGameDavidGameMode>();
				if (GRI != nullptr)
				{
					if (player->GetTeam() == ECaptureTeam::Red && isRed) 
					{
						GRI->AddRedScore();
						player->Myflag->SetState(ECaptureFlagState::InBase, this);
						player->Myflag = nullptr;
					}
					else if (player->GetTeam() == ECaptureTeam::Blue && !isRed)
					{
						GRI->AddBlueScore();
						player->Myflag->SetState(ECaptureFlagState::InBase, this);
						player->Myflag = nullptr;
					}
					
				}
			}
		}
	}
}



void ACaptureGameBaseScore::OnOverlapEnd(AActor * OverlappedActor, AActor * OtherActor)
{

}

// Called every frame
void ACaptureGameBaseScore::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

