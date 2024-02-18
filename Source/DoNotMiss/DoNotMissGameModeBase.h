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
	void GetHighScores(float& TimeSurvived, int32& EnemiesKilled) const;

	UFUNCTION(BlueprintCallable, Category = "Game Mode Functions")
	bool ResetHighScores();
protected:
	ADoNotMissGameModeBase();
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Set Up")
	TSubclassOf<class UDNM_MainMenuWidget> MainMenuWidget;
private:
	UPROPERTY()
	class USaveGame* SaveGameRef;
	
	UPROPERTY()
	class UDNM_SaveGame* ThisSaveGameRef;
	
	FString SaveGameSlot;
	void SetSaveGameRef();

	float TimeSurvivedHighScore;
	int32 EnemiesKilledHighScore;

	void CreateMainMenuWidget();
	UPROPERTY()
	UDNM_MainMenuWidget* MainMenuRef;
};
