// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "DoNotMissGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class DONOTMISS_API ADoNotMissGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	void GetHighScores(float& TimeSurvived, int32& EnemiesKilled, int32& AllTimeEnemiesKilled) const;

	UFUNCTION(BlueprintCallable, Category = "Game Mode Functions")
	bool ResetHighScores();
protected:
	ADoNotMissGameModeBase();
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Set Up")
	TSubclassOf<class UDNM_MainMenuWidget> MainMenuWidget;
private:
	UPROPERTY()
	class USaveGame* HighScoreSaveRef;
	
	UPROPERTY()
	class UDNM_SaveGame* ThisSaveGameRef;
	
	FString HighScoreSaveSlot;

	UPROPERTY()
	USaveGame* AllTimeSaveRef;

	UPROPERTY()
	UDNM_SaveGame* ThisAllTimeSaveRef;
	
	FString AllTimeSaveSlot;
	void SetSaveGameRef();

	float TimeSurvivedHighScore;
	int32 EnemiesKilledHighScore;
	int32 TotalEnemiesKilled;

	void CreateMainMenuWidget();
	UPROPERTY()
	UDNM_MainMenuWidget* MainMenuRef;
};
