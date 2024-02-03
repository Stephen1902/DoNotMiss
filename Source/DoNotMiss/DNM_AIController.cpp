// Copyright 2024 DME Games


#include "DNM_AIController.h"

#include "EnemyCharacterBase.h"
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

	GetWorld()->GetTimerManager().SetTimer(MoveDelayTimerHandle, this, &ADNM_AIController::StartToMove, TimeBeforeMove);
}

void ADNM_AIController::StartToMove()
{
	if (!TargetLocation.IsNearlyZero())
	{
		UE_LOG(LogTemp, Warning, TEXT("Moving to: %s"), *TargetLocation.ToString());
		MoveToLocation(TargetLocation, 100.f);
	}
}
