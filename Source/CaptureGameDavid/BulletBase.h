// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BulletBase.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UProjectileMovementComponent;
class UParticleSystem;
class AParticleSpawner;

UCLASS()
class CAPTUREGAMEDAVID_API ABulletBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABulletBase();

	UPROPERTY(EditAnywhere)
		USphereComponent* SphereComponet;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* MeshComponent;
	UPROPERTY(EditAnywhere)
		UProjectileMovementComponent* ProjectileMovementComponent;
	//UPROPERTY(Replicated)
	UParticleSystem* ParticleFX1;
	UFUNCTION()
		void OnExplode();

	UFUNCTION()
		void Delete();

	UFUNCTION()
		void ChangeMaterialTeam(bool Red);

	UFUNCTION(Reliable, NetMulticast)
		void NetMulticastExplode();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterial* BlueMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UMaterial* RedMaterial;

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
	FTimerHandle	TimerHandle_Fuse;
	float FuseTime;
	bool Color;
};
