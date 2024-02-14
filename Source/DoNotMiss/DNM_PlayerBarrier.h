// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DNM_PlayerBarrier.generated.h"

UCLASS()
class DONOTMISS_API ADNM_PlayerBarrier : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADNM_PlayerBarrier();

	void EnemyHasTakenAmmo(const int32 NumAmmoTaken);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayWidgetAnimation(const FText& AmmoText);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	USceneComponent* RootComp;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	class UBoxComponent* CollisionComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Set Up")
	class UWidgetComponent* BarrierWidget;
};
