// Copyright 2024 DME Games


#include "DNM_PlayerController.h"

void ADNM_PlayerController::GameHasStarted()
{
	OnGameStarted.Broadcast();
}
