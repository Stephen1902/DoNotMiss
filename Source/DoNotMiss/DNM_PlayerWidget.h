// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DNM_PlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class DONOTMISS_API UDNM_PlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void UpdateTimeSurvived(const FText& NewTime);
};
