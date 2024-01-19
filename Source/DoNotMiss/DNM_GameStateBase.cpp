// Copyright 2024 DME Games


#include "DNM_GameStateBase.h"

#include "EnemySpawnTargetPoint.h"
#include "Kismet/GameplayStatics.h"

ADNM_GameStateBase::ADNM_GameStateBase()
{
	// Set this to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentLevel = 0;
	LastSpawnPointUsed = -1;
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
	
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawnTargetPoint::StaticClass(), EnemySpawnTargetPoints);
	if (EnemySpawnTargetPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("GameStateBase failed to find any EnemySpawnTargetPoints"));
	}
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

		// Check if the time elapsed is greater than the time between spawning in the LevelInfo array and if the number of enemies alive for this level isn't exceeded 
		if (TimeSinceLastSpawn >= LevelInfo[CurrentLevel].TimeBetweenSpawns && CurrentEnemiesAlive < LevelInfo[CurrentLevel].MaxEnemiesAlive)
		{
			// Check for valid entries in the spawn point array
			if (EnemySpawnTargetPoints.Num() > 0)
			{
				const int32 RandomSpawnPoint = FMath::RandRange(1, EnemySpawnTargetPoints.Num()) - 1;
				// Check the random number hasn't chosen the same as previous, to avoid enemies spawning on the same path too close together
				if (RandomSpawnPoint != LastSpawnPointUsed)
				{
					UE_LOG(LogTemp, Warning, TEXT("Spawning using Random Point %s"), *EnemySpawnTargetPoints[RandomSpawnPoint]->GetName())
					LastSpawnPointUsed = RandomSpawnPoint;
					TimeSinceLastSpawn = 0.f;
					SpawnNewEnemy(EnemySpawnTargetPoints[RandomSpawnPoint]);
				}
			}
		}
	}
}

void ADNM_GameStateBase::SpawnNewEnemy(const AActor* SpawnPointToUse)
{
	if (SpawnPointToUse != nullptr)
	{
		
	}
}


