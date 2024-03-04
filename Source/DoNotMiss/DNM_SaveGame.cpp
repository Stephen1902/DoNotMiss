// Copyright 2024 DME Games


#include "DNM_SaveGame.h"

#include "Kismet/GameplayStatics.h"

// Sets default values
UDNM_SaveGame::UDNM_SaveGame()
{
	SavedTimeSurvived = 0.0f;
	SavedEnemiesKilled = 0;
	AllTimeEnemiesKilled = 0;

	bPlayMenuMusic = true;
	MenuMusicVolume = 1.0f;
	bPlayInGameSounds = true;
	InGameSoundVolume = 1.0f;

	AllTimeSaveSlot = "Slot1";
}

void UDNM_SaveGame::ResetSavedStats()
{
	SavedTimeSurvived = 0.0f;
	SavedEnemiesKilled = 0;
}

void UDNM_SaveGame::GetHighScores(float& TimeSurvived, int32& EnemiesKilled)
{
	TimeSurvived = SavedTimeSurvived;
	EnemiesKilled = SavedEnemiesKilled;
}

void UDNM_SaveGame::SetNewHighScores(float NewTimeSurvived, int32 NewEnemiesKilled)
{
	SavedTimeSurvived = NewTimeSurvived;
	SavedEnemiesKilled= NewEnemiesKilled;
}

void UDNM_SaveGame::GetAllTimeHighScores(int32& EnemiesKilled) const
{
	EnemiesKilled = AllTimeEnemiesKilled;
}

void UDNM_SaveGame::SetNewAllTimeTotal(int32 TotalToAdd)
{
	AllTimeEnemiesKilled += TotalToAdd;
}

void UDNM_SaveGame::GetInGameSettings(bool& PlayMenuMusic, float& MenuMusicLevel, bool& PlayInGameSounds, float& InGameLevel) const
{
	PlayMenuMusic = bPlayMenuMusic;
	MenuMusicLevel = MenuMusicVolume;
	PlayInGameSounds = bPlayInGameSounds;
	InGameLevel = InGameSoundVolume;
}

void UDNM_SaveGame::SetInGameSettings(bool PlayMenuMusic, float MenuMusicLevel, bool PlayInGameSounds, float InGameLevel)
{
	bPlayMenuMusic = PlayMenuMusic;
	MenuMusicVolume = MenuMusicLevel;
	bPlayInGameSounds = PlayInGameSounds;
	InGameSoundVolume = InGameLevel;

	if (UGameplayStatics::SaveGameToSlot(this, AllTimeSaveSlot, 1))
	{
		UE_LOG(LogTemp, Warning, TEXT("Settings Saved"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Settings failed to save"));
	}
}
