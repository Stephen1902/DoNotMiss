// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "DNM_GameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClockUpdated, float, NewClockTime);

USTRUCT(BlueprintType)
struct FLevelInfoStruct
{
	GENERATED_BODY()

	// Current game level
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Structs|Level Info Struct")
	int32 Level;

	// Number of enemies to spawn in this level
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Structs|Level Info Struct")
	int32 NumberOfEnemies;

	// Maximum number of enemies to be spawned at any time
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Structs|Level Info Struct")
	int32 MaxEnemiesAlive;

	// Time in seconds between enemy attempts to spawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Structs|Level Info Struct")
	float TimeBetweenSpawns;

	FLevelInfoStruct()
	{
		Level = 1;
		NumberOfEnemies = 10;
		MaxEnemiesAlive = 4;
		TimeBetweenSpawns = 1.0f;
	}
};

/**
 * 
 */
UCLASS()
class DONOTMISS_API ADNM_GameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	void SetGameIsRunning(const bool GameRunningIn);

	void EnemyHasDied();

	UPROPERTY()
	FOnClockUpdated OnClockUpdated;
protected:
	ADNM_GameStateBase();
	
	// Called When Game Starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Structs|Level Info Struct")
	TArray<FLevelInfoStruct> LevelInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Points")
	TArray<class AActor*> EnemySpawnTargetPoints;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemies")
	TArray<TSubclassOf<class ADNM_EnemyCharacterBase>> EnemiesToSpawn;

private:
	int32 CurrentLevel;
	int32 EnemiesLeftToSpawn;
	int32 CurrentEnemiesAlive;
	int32 LastSpawnPointUsed;
	float TimeSinceLastSpawn;

	void TryToSpawnNewEnemy(float DeltaSeconds);
	void SpawnNewEnemy(const AActor* SpawnPointToUse);
	
	bool bGameIsRunning = false;
	
	float TimePlayerAlive = 0.0f;
	void UpdateClock();
	FTimerHandle UpdateClockTimer;
};
