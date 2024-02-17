// Copyright 2024 DME Games


#include "DNM_MainMenuWidget.h"

#include "DoNotMissGameModeBase.h"
#include "Kismet/GameplayStatics.h"

void UDNM_MainMenuWidget::GetHighScoresFromGameBase()
{
	if (!GameModeRef)
	{
		GameModeRef = Cast<ADoNotMissGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	}

	GameModeRef->GetHighScores(TimeSurvivedHighScore, EnemiesKilledHighScore);
	DisplayHighScores(TimeSurvivedHighScore, EnemiesKilledHighScore);
}