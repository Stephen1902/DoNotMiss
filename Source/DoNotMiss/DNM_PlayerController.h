// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DNM_PlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameStarted);

/**
 * 
 */
UCLASS()
class DONOTMISS_API ADNM_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void GameHasStarted();

public:
	UPROPERTY()
	FOnGameStarted OnGameStarted;
};
