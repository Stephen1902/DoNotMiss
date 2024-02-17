// Copyright Epic Games, Inc. All Rights Reserved.


#include "DoNotMissGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "DNM_SaveGame.h"

void ADoNotMissGameModeBase::GetHighScores(float& TimeSurvived, int32& EnemiesKilled) const
{
	TimeSurvived = TimeSurvivedHighScore;
	EnemiesKilled = EnemiesKilledHighScore;
}

void ADoNotMissGameModeBase::SetSaveGameRef()
{
	// Check if a saved game already exists and if not, create it
	if (UGameplayStatics::DoesSaveGameExist(SaveGameSlot, 0))
	{
		SaveGameRef = UGameplayStatics::LoadGameFromSlot(SaveGameSlot, 0);
	}
	else
	{
		SaveGameRef = Cast<UDNM_SaveGame>(UGameplayStatics::CreateSaveGameObject(UDNM_SaveGame::StaticClass()));
	}

	// Cast to the specific instance of the SaveGame class for the HighScore variables
	ThisSaveGameRef = Cast<UDNM_SaveGame>(SaveGameRef);
	
	if (ThisSaveGameRef)
	{
		ThisSaveGameRef->GetHighScores(TimeSurvivedHighScore , EnemiesKilledHighScore);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get a Saved Game Reference in DoNotMissGameModeBase"));
	}
}
