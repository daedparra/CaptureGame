// Fill out your copyright notice in the Description page of Project Settings.

#include "CaptureGameDavidFlag.h"
#include "CaptureGameDavidCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TextRenderComponent.h"

// Sets default values
ACaptureGameDavidFlag::ACaptureGameDavidFlag()
	: State(ECaptureFlagState::Loose)
{
	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	RootComponent = CollisionComponent;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->AttachTo(RootComponent);

 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bReplicateMovement = true;
	BaseTransform = FTransform(FRotator(0,0,0),FVector(-420.0,-520.0,500.0),FVector(1,1,1));
}

// Called when the game starts or when spawned
void ACaptureGameDavidFlag::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ACaptureGameDavidFlag::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ACaptureGameDavidFlag::OnOverlapEnd);
}

void ACaptureGameDavidFlag::OnOverlapBegin(AActor * OverlappedActor, AActor * OtherActor)
{
	if (Cast<ACaptureGameDavidCharacter>(OtherActor)) 
	{
		if (State == ECaptureFlagState::Loose) {
			SetState(ECaptureFlagState::Carried, OtherActor);
		}
	}
	
}

void ACaptureGameDavidFlag::OnOverlapEnd(AActor * OverlappedActor, AActor * OtherActor)
{
	
}

// Called every frame
void ACaptureGameDavidFlag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

void ACaptureGameDavidFlag::SetState(ECaptureFlagState NewState, AActor* Pawn)
{
	switch (State)
	{
		case ECaptureFlagState::Carried:
		{
			SetActorRelativeLocation(Pawn->GetActorLocation());

			FDetachmentTransformRules Rules(EDetachmentRule::KeepRelative, false);
			DetachFromActor(Rules);

			SetActorEnableCollision(true);

			break;
		}
	}

	switch (NewState)
	{
		case ECaptureFlagState::Carried:
		{

			FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);
			AttachToActor(Pawn, Rules, FName(TEXT("lamp")));
			SetActorRelativeLocation(FVector(0.0f, -50.0f, 0.0f));
			SetActorEnableCollision(false);
			ACaptureGameDavidCharacter* player = Cast<ACaptureGameDavidCharacter>(Pawn);
			player->SetCarriedFlag(this);

			break;
		}

		case ECaptureFlagState::InBase:
		{
			SetActorTransform(BaseTransform);
			NewState = ECaptureFlagState::Loose;
			break;
		}

		case ECaptureFlagState::Loose:
		{
			break;
		}
	}

	State = NewState;
}
