// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DNM_MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class DONOTMISS_API UDNM_MainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "About Tab")
	void GetHighScoresFromGameBase();
	
	UFUNCTION(BlueprintImplementableEvent)
	void DisplayHighScores(const float& TimeSurvived, const int32& EnemiesKilled);
	
private:
	UPROPERTY()
	class ADoNotMissGameModeBase* GameModeRef;
	
	float TimeSurvivedHighScore;
	int32 EnemiesKilledHighScore;
};
