// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CaptureGameTypes.h"

#include "CaptureGameDavidCharacter.generated.h"

class ABulletBase;
class ACaptureGameDavidFlag;
UCLASS(config=Game)
class ACaptureGameDavidCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	ACaptureGameDavidCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UFUNCTION()
	ECaptureTeam GetTeam();

	UFUNCTION()
		void SetTeam(ECaptureTeam team);

	UFUNCTION()
		void OnRep_Team();

	UPROPERTY(Transient, ReplicatedUsing = OnRep_Team)
		ECaptureTeam mTeam;

	UPROPERTY(EditAnywhere, Category = Grenade)
		TSubclassOf<ABulletBase> GrenadeToUse;

protected:

	void OnThrowGrenade();
	UFUNCTION(Server, WithValidation, Reliable)
		void ServerThrowGrenade();

	UPROPERTY(Replicated)
		int numGrenades;

	UFUNCTION(Reliable, NetMulticast)
		void NetMulticastNum();

	UFUNCTION()
		void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
		void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	void SetCarriedFlag(AActor* flag);
	ACaptureGameDavidFlag* Myflag;
	int Health=5;

	UPROPERTY(EditAnywhere, Category = "Disable")
	bool HideInGame;

	UFUNCTION(BlueprintCallable, Category = "Disable")
	void DisableActor(bool toHide);

	TArray<AActor*> PlayerStartArray;

};

