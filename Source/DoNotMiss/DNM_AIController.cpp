// Copyright 2024 DME Games

#include "DNM_AIController.h"

#include "DNM_PlayerController.h"
#include "Kismet/GameplayStatics.h"

ADNM_AIController::ADNM_AIController()
{
	PrimaryActorTick.bCanEverTick = true;

	TargetLocation = FVector::ZeroVector;
}

void ADNM_AIController::BeginPlay()
{
	Super::BeginPlay();

	TargetLocation = UGameplayStatics::GetPlayerPawn(GetWorld(), 0)->GetActorLocation();

	PlayerControllerRef = Cast<ADNM_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerControllerRef)
	{
		PlayerControllerRef->OnGameRunningChanged.AddDynamic(this, &ADNM_AIController::SetGameIsRunning);
	}

	GetWorld()->GetTimerManager().SetTimer(MoveDelayTimerHandle, this, &ADNM_AIController::StartToMove, TimeBeforeMove);
}

void ADNM_AIController::StartToMove()
{
	if (!TargetLocation.IsNearlyZero())
	{
		MoveToLocation(TargetLocation, 200.f);
	}
}

void ADNM_AIController::SetGameIsRunning(const bool GameStatusIn)
{
	if (GameStatusIn == true)
	{
		StartToMove();
	}
	else
	{
		StopMovement();
	}
}
