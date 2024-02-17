// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DNM_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class DONOTMISS_API UDNM_SaveGame : public USaveGame
{
	GENERATED_BODY()

protected:
	UDNM_SaveGame();
	
	UPROPERTY(EditAnywhere, Category = "Saved Game Options")
	float SavedTimeSurvived;

	UPROPERTY(EditAnywhere, Category = "Saved Game Options")
	int32 SavedEnemiesKilled;

public:
	void ResetSavedStats();

	void GetHighScores(float& TimeSurvived, int32& EnemiesKilled);
};
