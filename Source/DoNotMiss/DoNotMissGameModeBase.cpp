// Copyright Epic Games, Inc. All Rights Reserved.


#include "DoNotMissGameModeBase.h"
#include "DNM_MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "DNM_SaveGame.h"
#include "Blueprint/UserWidget.h"

/* Sets default variables */
ADoNotMissGameModeBase::ADoNotMissGameModeBase()
{
	PrimaryActorTick.bCanEverTick = false;

	SaveGameSlot = "Slot0";
}


void ADoNotMissGameModeBase::GetHighScores(float& TimeSurvived, int32& EnemiesKilled) const
{
	TimeSurvived = TimeSurvivedHighScore;
	EnemiesKilled = EnemiesKilledHighScore;
}

bool ADoNotMissGameModeBase::ResetHighScores()
{
	if (ThisSaveGameRef)
	{
		ThisSaveGameRef->SetNewHighScores(0.0f, 0);

		if (UGameplayStatics::SaveGameToSlot(ThisSaveGameRef, SaveGameSlot, 0))
		{
			UE_LOG(LogTemp, Warning, TEXT("Successfully Reset Scores"));
			TimeSurvivedHighScore = 0.0f;
			EnemiesKilledHighScore = 0;
			return true;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Failed to Reset Scores"));
	return false;
}


void ADoNotMissGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SetSaveGameRef();
	CreateMainMenuWidget();
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
		UE_LOG(LogTemp, Warning, TEXT("TimeSurvivedHighScore is %s, EnemiesKilledHighScore is %i"), *FString::SanitizeFloat(TimeSurvivedHighScore), EnemiesKilledHighScore);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get a Saved Game Reference in DoNotMissGameModeBase"));
	}
}

void ADoNotMissGameModeBase::CreateMainMenuWidget()
{
	if (MainMenuWidget)
	{
		MainMenuRef = CreateWidget<UDNM_MainMenuWidget>(UGameplayStatics::GetPlayerController(GetWorld(), 0), MainMenuWidget);
		MainMenuRef->AddToViewport();
		MainMenuRef->SetGameModeRef(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("MainMenuWidget is not set in DoNotMissGameModeBase"));
	}
}
