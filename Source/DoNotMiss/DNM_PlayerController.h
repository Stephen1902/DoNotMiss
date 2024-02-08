// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DNM_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DONOTMISS_API ADNM_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void UpdateWidgetClock(const float NewTime);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Set Up")
	TSubclassOf<class UDNM_PlayerWidget> PlayerWidget;
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void GameHasStarted();

	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class ADNM_GameStateBase* GameStateRef;

	void SetGameStateRef();

	UPROPERTY()
	UDNM_PlayerWidget* PlayerWidgetRef;

	void AddPlayerWidget();
	
	bool bGameIsRunning = false;
};
