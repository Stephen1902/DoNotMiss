// Copyright 2024 DME Games


#include "EnemySpawnTargetPoint.h"

#include "Components/ArrowComponent.h"

AEnemySpawnTargetPoint::AEnemySpawnTargetPoint()
{
	ArrowComp = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow Comp"));
	ArrowComp->SetupAttachment(RootComponent);
}
