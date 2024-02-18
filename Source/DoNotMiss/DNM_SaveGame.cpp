// Copyright 2024 DME Games


#include "DNM_SaveGame.h"

// Sets default values
UDNM_SaveGame::UDNM_SaveGame()
{
	SavedTimeSurvived = 0.0f;
	SavedEnemiesKilled = 0;
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
