// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CaptureGameTypes.h"
#include "CaptureGameDavidFlag.generated.h"

class UStaticMeshComponent;
class UShapeComponent;
class ACaptureGameDavidCharacter;
class UMaterial;

enum class ECaptureFlagState : uint8
{
	Carried = 0,
	InBase,
	Loose
};

UCLASS()
class CAPTUREGAMEDAVID_API ACaptureGameDavidFlag : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACaptureGameDavidFlag();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UShapeComponent* CollisionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterial* CapturedMaterial;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
		void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetState(ECaptureFlagState NewState, AActor* Pawn);

private:
	ECaptureFlagState State;
	float CaptureTimeCurrent;
	ECaptureTeam mTeam;
	FTransform BaseTransform;
};
