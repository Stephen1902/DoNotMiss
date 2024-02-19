// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DNM_EnemyCharacterBase.generated.h"

UCLASS()
class DONOTMISS_API ADNM_EnemyCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADNM_EnemyCharacterBase();

	// Called when a projectile hits this enemy
	void DealWithProjectile(class ADNM_ProjectileBase* ProjectileThatHit, float DamageCaused);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The amount of health this enemy starts the game with
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	float StartingHealth; 

	//  After spawning, how long before the enemy moves towards the player
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	float DelayBeforeMoving;

	// When the enemy reaches the player barrier, how many ammo does it remove
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	int32 AmmoTakenPerHit;

	// How long between the enemy taking more ammo
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	float DelayBetweenAmmoTake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	class UWidgetComponent* EnemyHealthWidgetComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	FLinearColor HealthBarColour;

	UFUNCTION(BlueprintImplementableEvent)
	void OnHealthChanged(const float& HealthPercentage);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float CurrentHealth;
	void EnemyHasDied();
	
	UPROPERTY()
	TArray<ADNM_ProjectileBase*> ProjectilesThatHit;
	//TMap<ADNM_ProjectileBase*, float> ProjectilesThatHit;

private:
	FTimerHandle TakeAmmoTimer;
	void TakeEnemyAmmo();

	UPROPERTY()
	class ADNM_PlayerBarrier* PlayerBarrierRef;

	UPROPERTY()
	class ADNM_PlayerController* PlayerControllerRef;

	UFUNCTION()
	void SetGameIsRunning(const bool GameRunningIn);
	
	UFUNCTION()
	void ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
};

