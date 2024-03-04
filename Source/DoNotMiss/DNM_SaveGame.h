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
	
	UPROPERTY(VisibleAnywhere, Category = "Saved Game Options")
	float SavedTimeSurvived;

	UPROPERTY(VisibleAnywhere, Category = "Saved Game Options")
	int32 SavedEnemiesKilled;

	UPROPERTY(VisibleAnywhere, Category = "Saved Game Options")
	int32 AllTimeEnemiesKilled;

	UPROPERTY(VisibleAnywhere, Category = "Saved Game Options")
	bool bPlayMenuMusic;

	UPROPERTY(VisibleAnywhere, Category = "Saved Game Options")
	float MenuMusicVolume;

	UPROPERTY(VisibleAnywhere, Category = "Saved Game Options")
	bool bPlayInGameSounds;
	
	UPROPERTY(VisibleAnywhere, Category = "Saved Game Options")
	float InGameSoundVolume;
	
public:
	void ResetSavedStats();

	void GetHighScores(float& TimeSurvived, int32& EnemiesKilled);
	void SetNewHighScores(float NewTimeSurvived, int32 NewEnemiesKilled);

	void GetAllTimeHighScores(int32& EnemiesKilled) const;
	void SetNewAllTimeTotal(int32 TotalToAdd);

	void GetInGameSettings(bool& PlayMenuMusic, float& MenuMusicLevel, bool& PlayInGameSounds, float& InGameLevel) const;
	void SetInGameSettings(bool PlayMenuMusic, float MenuMusicLevel, bool PlayInGameSounds, float InGameLevel);

private:
	FString AllTimeSaveSlot;
};
