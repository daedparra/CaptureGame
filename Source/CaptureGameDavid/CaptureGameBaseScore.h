// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CaptureGameBaseScore.generated.h"

class UShapeComponent;
class UTextRenderComponent;

UCLASS()
class CAPTUREGAMEDAVID_API ACaptureGameBaseScore : public AActor
{
	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	ACaptureGameBaseScore();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UShapeComponent* CollisionComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UTextRenderComponent* TextComponent;

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool isRed;
};
