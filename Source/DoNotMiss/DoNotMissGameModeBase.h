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
	void GetUserSettings(bool& PlayMenuMusicOUT, float& MenuMusicVolumeOUT, bool& PlayInGameSoundsOUT, float& InGameSoundVolumeOUT) const;

	UFUNCTION(BlueprintCallable, Category = "Game Mode Functions")
	bool ResetHighScores();

	UFUNCTION(BlueprintCallable, Category = "Game Mode Functions")
	bool SaveSettings();

	UFUNCTION(BlueprintCallable, Category = "Game Mode Functions")
	void MenuMusicStateChanged(bool NewMusicStateIn);

	UFUNCTION(BlueprintCallable, Category = "Game Mode Functions")
	void AdjustMenuMusicVolume(const float VolumeLevelIn);

	UFUNCTION(BlueprintCallable, Category = "Game Mode Functions")
	void InGameSoundStateChanged(bool NewSoundStateIn);

	UFUNCTION(BlueprintCallable, Category = "Game Mode Functions")
	void AdjustInGameSoundVolume(const float VolumeLevelIn);
	
protected:
	ADoNotMissGameModeBase();
	
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Set Up")
	TSubclassOf<class UDNM_MainMenuWidget> MainMenuWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	class USoundCue* MainMenuMusic; 
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

	// Player stats variables
	float TimeSurvivedHighScore;
	int32 EnemiesKilledHighScore;
	int32 TotalEnemiesKilled;

	// Game setting variables
	bool bPlayMenuMusic;
	float MenuMusicVolume;
	bool bPlayInGameSounds;
	float InGameSoundVolume;
	void PlayMenuMusic();

	void CreateMainMenuWidget();
	UPROPERTY()
	UDNM_MainMenuWidget* MainMenuRef;

	UPROPERTY()
	UAudioComponent* AudioComponent;
};
