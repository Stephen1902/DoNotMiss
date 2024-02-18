// Copyright 2024 DME Games


#include "DNM_GameOverWidget.h"

void UDNM_GameOverWidget::SetTimeSurvivedAsText(FString& TimeAsString, const FText& EnemiesKilledAsText)
{
	if (!TimeAsString.IsEmpty() && !EnemiesKilledAsText.IsEmpty())
	{
		TimeAsString = TimeAsString + " SECONDS";
		OnGameOver(FText::FromString(TimeAsString), EnemiesKilledAsText);
	}
}

void UDNM_GameOverWidget::NewHighScore()
{
	OnNewHighScore();
}
