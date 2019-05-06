// Fill out your copyright notice in the Description page of Project Settings.

#include "BulletBase.h"
#include "CaptureGameDavidCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "CaptureGameTypes.h"

// Sets default values
ABulletBase::ABulletBase()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	SphereComponet = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

	RootComponent = SphereComponet;
	MeshComponent->AttachTo(RootComponent);
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->SetUpdatedComponent(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem> Particle(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	ParticleFX1 = Particle.Object;
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	FuseTime = 1.0f;

	//Replicates the grenade on client
	bReplicates = true;
	bReplicateMovement = true;

}

//Calling this function in a certain time
void ABulletBase::OnExplode()
{
	NetMulticastExplode();

	Destroy();


	//GetWorld()->SpawnActor(Particle, &GetActorLocation(), &GetActorRotation());
}

void ABulletBase::ChangeMaterialTeam(bool Red)
{
	if (Red)
	{
		MeshComponent->SetMaterial(0, RedMaterial);
		Color = Red;
	}
	else
	{
		MeshComponent->SetMaterial(0, BlueMaterial);
		Color = Red;
	}
}

// Called when the game starts or when spawned
void ABulletBase::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ABulletBase::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ABulletBase::OnOverlapEnd);
	if (Role == ROLE_Authority)
	{
		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			World->GetTimerManager().SetTimer(TimerHandle_Fuse, this, &ABulletBase::Delete, FuseTime);
		}
	}
}

void ABulletBase::OnOverlapBegin(AActor * OverlappedActor, AActor * OtherActor)
{
	ACaptureGameDavidCharacter* Player = Cast<ACaptureGameDavidCharacter>(OtherActor);
	if (Player != nullptr) {
		/*if (Player->GetTeam() == ECaptureTeam::Red && Color)
		{*/
			Player->Health--;
			OnExplode();
		//}
		//else if(Player->GetTeam() == ECaptureTeam::Blue && !Color)
		//{
		//	Player->Health--;
		//	OnExplode();
		//}
	}
}

void ABulletBase::OnOverlapEnd(AActor * OverlappedActor, AActor * OtherActor)
{

}

void ABulletBase::Delete() 
{
	Destroy();
}

// Called every frame
void ABulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

//Cast the particle effect via server
void ABulletBase::NetMulticastExplode_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleFX1, GetActorLocation());
}


