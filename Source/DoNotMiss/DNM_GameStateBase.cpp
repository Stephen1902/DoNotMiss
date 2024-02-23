// Copyright 2024 DME Games


#include "DNM_GameStateBase.h"
#include "DNM_EnemyCharacterBase.h"
#include "EnemySpawnTargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "DNM_PlayerController.h"

ADNM_GameStateBase::ADNM_GameStateBase()
{
	// Set this to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CurrentLevel = 0;
	LastSpawnPointUsed = -1;
	CurrentEnemiesAlive = 0;
	EnemiesLeftToSpawn = 0;
	TimeSinceLastSpawn = 0.0f;
}

void ADNM_GameStateBase::BeginPlay()
{
	Super::BeginPlay();

	if (LevelInfo.Num() > 0)
	{
		EnemiesLeftToSpawn = LevelInfo[CurrentLevel].NumberOfEnemies;
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

	PlayerControllerRef = Cast<ADNM_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerControllerRef)
	{
		PlayerControllerRef->OnGameRunningChanged.AddDynamic(this, &ADNM_GameStateBase::SetGameIsRunning);
	}
	
}

void ADNM_GameStateBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bGameIsRunning)
	{
		TryToSpawnNewEnemy(DeltaSeconds);
	}
}

void ADNM_GameStateBase::TryToSpawnNewEnemy(float DeltaSeconds)
{
	// Make sure there is information in the LevelInfo and EnemiesToSpawn arrays
	if (LevelInfo.Num() > 0 && EnemiesToSpawn.Num() > 0)
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
					LastSpawnPointUsed = RandomSpawnPoint;
					TimeSinceLastSpawn = 0.f;
					CurrentEnemiesAlive += 1;
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
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		const int32 RandomEnemyToSpawn = FMath::FRandRange(0, EnemiesToSpawn.Num() - 1);
		GetWorld()->SpawnActor<ADNM_EnemyCharacterBase>(EnemiesToSpawn[RandomEnemyToSpawn], SpawnPointToUse->GetActorLocation(), SpawnPointToUse->GetActorRotation(), SpawnParameters);
	}
}

void ADNM_GameStateBase::UpdateClock()
{
	TimePlayerAlive += 0.1f;
	OnClockUpdated.Broadcast(TimePlayerAlive);
}

void ADNM_GameStateBase::SetGameIsRunning(const bool GameRunningIn)
{
	bGameIsRunning = GameRunningIn;

	if (GetWorld()->GetTimerManager().IsTimerActive(UpdateClockTimer))
	{
		if (!bGameIsRunning)
		{
			GetWorld()->GetTimerManager().PauseTimer(UpdateClockTimer);
		}
		else
		{
			GetWorld()->GetTimerManager().UnPauseTimer(UpdateClockTimer);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(UpdateClockTimer, this, &ADNM_GameStateBase::UpdateClock, 0.1f, true, 0.1f);
	}
}

void ADNM_GameStateBase::EnemyHasDied()
{
	//  Randomly generate a chance to spawn an extra enemy
	if (CurrentEnemiesAlive == LevelInfo[CurrentLevel].MaxEnemiesAlive)
	{
		const float RandomChance = FMath::FRandRange(0.f, 100.f);
		// If the random chance is less than the spawn chance, remove an additional enemy count
		if (RandomChance < LevelInfo[CurrentLevel].ChanceOfAdditionalEnemySpawn)
		{
			CurrentEnemiesAlive -= 1;	
		}
	}
	else
	{
		CurrentEnemiesAlive -= 1;	
	}
	
}
