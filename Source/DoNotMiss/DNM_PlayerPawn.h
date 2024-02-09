// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DNM_PlayerPawn.generated.h"

class ADNM_WeaponBase;

UCLASS()
class DONOTMISS_API ADNM_PlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ADNM_PlayerPawn();

	USkeletalMeshComponent* GetSkeletalMeshComp() const { return SkeletalMeshComponent; }

	void SetGameIsRunning(const bool GameIsRunningIn) { bGameIsRunning = GameIsRunningIn; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	class UCapsuleComponent* CapsuleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	class UCameraComponent* CameraComponent;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	UPROPERTY()
	APlayerController* PlayerController;

	void RotatePlayer(float DeltaTime);
	
	bool bGameIsRunning;
};

