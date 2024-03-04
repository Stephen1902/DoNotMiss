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

public:
	void SetGameModeRef(class ADoNotMissGameModeBase* GameModeIn);
protected:
	UFUNCTION(BlueprintCallable, Category = "About Tab")
	void GetHighScoresFromGameBase();
	
	UFUNCTION(BlueprintImplementableEvent)
	void DisplayHighScores(const float& TimeSurvived, const int32& EnemiesKilled, const int32& AllTimeEnemies);

	UFUNCTION(BlueprintCallable, Category = "About Tab")
	void GetGameSettingsFromGameBase();
	
	UFUNCTION(BlueprintImplementableEvent)
	void SendInGameSettings (const bool PlayMenuMusic, const float& MenuMusicVolume, const bool PlayInGameSounds, const float& InGameSoundVolume);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Mode")
	ADoNotMissGameModeBase* GameModeRef;

private:
	// High score variables
	float TimeSurvivedHighScore;
	int32 EnemiesKilledHighScore;
	int32 AllTimeEnemiesKilled;

	// Game setting variables
	bool bPlayMenuMusic;
	float MenuMusicVol;
	bool bPlayInGameSounds;
	float InGameSoundVol;
};
