// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "CaptureGameDavidCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "UnrealNetwork.h"
#include "BulletBase.h"
#include "CaptureGameDavidFlag.h"
#include "CaptureGameDavidGameState.h"
#include "CaptureGameDavidGameMode.h"
#include "GameFramework/PlayerStart.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

//////////////////////////////////////////////////////////////////////////
// ACaptureGameDavidCharacter

ACaptureGameDavidCharacter::ACaptureGameDavidCharacter()
	: mTeam(ECaptureTeam::Invalid)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	numGrenades = 5; 
	Myflag = nullptr;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACaptureGameDavidCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACaptureGameDavidCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACaptureGameDavidCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACaptureGameDavidCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACaptureGameDavidCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACaptureGameDavidCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACaptureGameDavidCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ACaptureGameDavidCharacter::OnResetVR);

	PlayerInputComponent->BindAction("ThrowGrenade", IE_Pressed, this, &ACaptureGameDavidCharacter::OnThrowGrenade);

}

void ACaptureGameDavidCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ACaptureGameDavidCharacter, mTeam);
	DOREPLIFETIME(ACaptureGameDavidCharacter, numGrenades);

}


ECaptureTeam ACaptureGameDavidCharacter::GetTeam()
{
	return mTeam;
}

void ACaptureGameDavidCharacter::SetTeam(ECaptureTeam team)
{
	mTeam = team;

	OnRep_Team();
}

void ACaptureGameDavidCharacter::Tick(float DeltaTime)
{

	Super::Tick(DeltaTime);
	if (Health <= 0) {
		if (Myflag != nullptr)
		{
			Myflag->SetState(ECaptureFlagState::Loose, this);
			Myflag = nullptr;
		}

		AController* PlayerController = GetController();

		//this->DisableInput(PlayerController);
		//HideInGame = true;
		//DisableActor(HideInGame);

		UWorld* World = GetWorld();

		if (World != nullptr)
		{
			ACaptureGameDavidGameMode* GRI = World->GetAuthGameMode<ACaptureGameDavidGameMode>();

			if (GRI != nullptr)
			{
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStartArray);
				for (int32 y = 0; y < PlayerStartArray.Num(); y++)
				{
					APlayerStart* Player = Cast<APlayerStart>(PlayerStartArray[y]);
					int32 tag = FCString::Atoi(*Player->PlayerStartTag.ToString());
					if (tag == 0 && GetTeam() == ECaptureTeam::Red || tag == 1 && GetTeam() == ECaptureTeam::Red) 
					{
						GRI->RespawnPlayer(PlayerController, Player);
						Health = 5;
						//HideInGame = false;
						//DisableActor(HideInGame);
						break;
					}
					if (tag == 2 && GetTeam() == ECaptureTeam::Blue || tag == 3 && GetTeam() == ECaptureTeam::Blue)
					{
						GRI->RespawnPlayer(PlayerController, Player);
						Health = 5;
						//HideInGame = false;
						//DisableActor(HideInGame);
						break;
					}
				}
				
			}
		}

	}

}


void ACaptureGameDavidCharacter::OnRep_Team()
{
	UMaterialInstanceDynamic* MaterialInstanceDynamic = GetMesh()->CreateAndSetMaterialInstanceDynamic(0);

	if (mTeam == ECaptureTeam::Red)
	{
		MaterialInstanceDynamic->SetVectorParameterValue(FName("BodyColor"), FLinearColor(1.0f, 0.0f, 0.0f));
	}
	else
	{
		MaterialInstanceDynamic->SetVectorParameterValue(FName("BodyColor"), FLinearColor(0.0f, 0.0f, 1.0f));
	}
}

void ACaptureGameDavidCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ACaptureGameDavidCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void ACaptureGameDavidCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void ACaptureGameDavidCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACaptureGameDavidCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACaptureGameDavidCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACaptureGameDavidCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}


//Function to call every time he press G
void ACaptureGameDavidCharacter::OnThrowGrenade()
{
	/*if (numGrenades > 0)
	{*/
		ServerThrowGrenade();
	/*}*/
}


void ACaptureGameDavidCharacter::SetCarriedFlag(AActor* flag) {
	Myflag = Cast<ACaptureGameDavidFlag>(flag);
}

void ACaptureGameDavidCharacter::DisableActor(bool toHide)
{
	SetActorHiddenInGame(toHide);

	// Disables collision components
	SetActorEnableCollision(false);

	// Stops the Actor from ticking
	SetActorTickEnabled(false);
}

//Multicast via networking the number of grenades the player has
void ACaptureGameDavidCharacter::NetMulticastNum_Implementation() {
	numGrenades--;
}

bool ACaptureGameDavidCharacter::ServerThrowGrenade_Validate()
{
	return true;
}

void ACaptureGameDavidCharacter::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ACaptureGameDavidCharacter::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &ACaptureGameDavidCharacter::OnOverlapEnd);

	if (Role == ROLE_Authority)
	{
		UWorld* World = GetWorld();

		if (World != nullptr)
		{
			ACaptureGameDavidGameState* GRI = World->GetGameState<ACaptureGameDavidGameState>();

			if (GRI != nullptr)
			{
				GRI->AddToTeam(this);
			}
		}
	}
}

void ACaptureGameDavidCharacter::OnOverlapBegin(AActor * OverlappedActor, AActor * OtherActor)
{
	
}

void ACaptureGameDavidCharacter::OnOverlapEnd(AActor * OverlappedActor, AActor * OtherActor)
{

}

//Throwing grenades through server
void ACaptureGameDavidCharacter::ServerThrowGrenade_Implementation()
{
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}
	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = this;
	SpawnParam.Instigator = this;

	FVector StartPosition = GetActorLocation();
	StartPosition += GetActorForwardVector() * 150.0f;
	StartPosition.Z += 40.0f;

	FRotator StartRotation = GetActorForwardVector().Rotation();
	if(mTeam == ECaptureTeam::Red)
		GrenadeToUse->GetDefaultObject<ABulletBase>()->ChangeMaterialTeam(true);
	else
		GrenadeToUse->GetDefaultObject<ABulletBase>()->ChangeMaterialTeam(false);
	world->SpawnActor(GrenadeToUse, &StartPosition, &StartRotation, SpawnParam);
	NetMulticastNum();
}