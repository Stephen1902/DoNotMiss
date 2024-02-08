// Copyright 2024 DME Games

#include "DNM_PlayerController.h"

#include "DNM_GameStateBase.h"
#include "DNM_PlayerWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetStringLibrary.h"

void ADNM_PlayerController::GameHasStarted()
{
	bGameIsRunning = true;
	//OnGameStarted.Broadcast();

	if (GameStateRef)
	{
		GameStateRef->SetGameIsRunning(true);
	}
}

void ADNM_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetGameStateRef();
	AddPlayerWidget();
}

void ADNM_PlayerController::SetGameStateRef()
{
	GameStateRef = Cast<ADNM_GameStateBase>(UGameplayStatics::GetGameState(GetWorld()));

	if (GameStateRef)
	{
		GameStateRef->OnClockUpdated.AddDynamic(this, &ADNM_PlayerController::UpdateWidgetClock);		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller failed to set the GameStateRef"));
	}
}

void ADNM_PlayerController::AddPlayerWidget()
{
	if (PlayerWidget)
	{
		PlayerWidgetRef = CreateWidget<UDNM_PlayerWidget>(this, PlayerWidget);
		PlayerWidgetRef->AddToViewport();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller does not have a valid player widget set"));
	}
}

void ADNM_PlayerController::UpdateWidgetClock(const float NewTime)
{
	if (PlayerWidgetRef)
	{
		FString NewTimeAsString = FString::SanitizeFloat(NewTime, 2);
		const int32 DecimalPointPos = NewTimeAsString.Find(".");
		NewTimeAsString = NewTimeAsString.Left(DecimalPointPos + 2) + "0";
		PlayerWidgetRef->UpdateTimeSurvived(FText::FromString(NewTimeAsString));
	}
}
