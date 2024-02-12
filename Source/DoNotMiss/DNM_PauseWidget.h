// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DNM_PauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class DONOTMISS_API UDNM_PauseWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetPlayerControllerRef(class ADNM_PlayerController* ControllerRefIn);
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Player Controller")
	ADNM_PlayerController* PlayerControllerRef;
};
