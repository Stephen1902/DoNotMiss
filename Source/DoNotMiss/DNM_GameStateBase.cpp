// Copyright 2024 DME Games


#include "DNM_GameStateBase.h"

ADNM_GameStateBase::ADNM_GameStateBase()
{
	// Set this to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentLevel = 0;	
}

void ADNM_GameStateBase::BeginPlay()
{
	Super::BeginPlay();

	if (LevelInfo.Num() > 0)
	{
		EnemiesLeftToSpawn = LevelInfo[CurrentLevel].NumberOfEnemies;;
		TimeSinceLastSpawn = LevelInfo[CurrentLevel].TimeBetweenSpawns;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("The LevelInfo array contains no entries in GameStateBase."));
	}

	CurrentEnemiesAlive = 0;
}

void ADNM_GameStateBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	TryToSpawnNewEnemy(DeltaSeconds);

}

void ADNM_GameStateBase::TryToSpawnNewEnemy(float DeltaSeconds)
{
	// Make sure there is information in the array
	if (LevelInfo.Num() > 0)
	{
		TimeSinceLastSpawn += DeltaSeconds;

		if (TimeSinceLastSpawn >= LevelInfo[CurrentLevel].TimeBetweenSpawns && CurrentEnemiesAlive < LevelInfo[CurrentLevel].MaxEnemiesAlive)
		{
			TimeSinceLastSpawn = 0.f;
			SpawnNewEnemy();
		}
	}
}

void ADNM_GameStateBase::SpawnNewEnemy()
{
	
}


