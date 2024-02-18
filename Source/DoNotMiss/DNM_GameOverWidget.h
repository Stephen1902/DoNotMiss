// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DNM_GameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class DONOTMISS_API UDNM_GameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetTimeSurvivedAsText(FString& TimeAsString, const FText& EnemiesKilledAsText);

	void NewHighScore();
protected:
	UFUNCTION(BlueprintImplementableEvent)
	void OnGameOver(const FText& TimeSurvivedText, const FText& EnemiesKilledText);

	UFUNCTION(BlueprintImplementableEvent)
	void OnNewHighScore();
};
