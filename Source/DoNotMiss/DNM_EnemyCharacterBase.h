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
	void DealWithProjectile(class ADNM_ProjectileBase* ProjectileThatHit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The amount of health this enemy starts the game with
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	float StartingHealth; 

	//  After spawning, how long before the enemy moves towards the player
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	float DelayBeforeMoving;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float CurrentHealth;
	void EnemyHasDied();
	
	UPROPERTY()
	TArray<ADNM_ProjectileBase*> ProjectilesThatHit;
	
};

