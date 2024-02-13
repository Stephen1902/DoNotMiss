// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DNM_AIController.generated.h"

/**
 * 
 */
UCLASS()
class DONOTMISS_API ADNM_AIController : public AAIController
{
	GENERATED_BODY()

	// Set the default values for this class
	ADNM_AIController();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	FVector TargetLocation;

	// There's a pause after spawning before the controller possesses the character.  Set a small delay to offset this.
	UPROPERTY()
	FTimerHandle MoveDelayTimerHandle;
	float TimeBeforeMove = 0.2f;	

	UFUNCTION()
	void StartToMove();
	
	UPROPERTY()
	class ADNM_PlayerController* PlayerControllerRef;
	
	UFUNCTION()
	void SetGameIsRunning(const bool GameStatusIn);
};
