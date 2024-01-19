// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "EnemySpawnTargetPoint.generated.h"

/**
 * 
 */
UCLASS()
class DONOTMISS_API AEnemySpawnTargetPoint : public ATargetPoint
{
	GENERATED_BODY()

protected:
	AEnemySpawnTargetPoint();
	
	UPROPERTY(VisibleAnywhere, Category = "Arrow Comp")
	UArrowComponent* ArrowComp;
	
};
