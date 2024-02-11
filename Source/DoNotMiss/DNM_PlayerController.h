// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DNM_PlayerController.generated.h"

class ADNM_WeaponBase;
/**
 * 
 */
UCLASS()
class DONOTMISS_API ADNM_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void UpdateWidgetClock(const float NewTime);

	void ReturnPlayerBullet(int32 BulletNumber);

	UFUNCTION(BlueprintCallable, Category = "Pause Menu")
	void TogglePauseWidget();
	
protected:
	ADNM_PlayerController();
	
	UPROPERTY(EditDefaultsOnly, Category = "Set Up")
	TSubclassOf<class UDNM_PlayerWidget> PlayerWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Set Up")
	TSubclassOf<class UDNM_PauseWidget> PauseWidget;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	TSubclassOf<ADNM_WeaponBase> PistolBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	TSubclassOf<ADNM_WeaponBase> ShotgunBP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	TSubclassOf<ADNM_WeaponBase> RifleBP;
	
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void GameHasStarted();

	// Called when the game starts
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Set up the player input component
	virtual void SetupInputComponent() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	ADNM_WeaponBase* PlayerWeapon;
private:
	UPROPERTY()
	class ADNM_GameStateBase* GameStateRef;

	void SetGameStateRef();

	UPROPERTY()
	UDNM_PlayerWidget* PlayerWidgetRef;
	void AddPlayerWidget();

	UPROPERTY()
	UDNM_PauseWidget* PauseWidgetRef;
	bool bGameIsRunning;
	bool bGameIsPaused;

	int32 PlayerBullets;
	void SpawnPlayerWeapon(TSubclassOf<ADNM_WeaponBase> WeaponToUse, FName SocketToUse);
	float TimeBetweenFiring;
	float TimeSinceLastFired;
	void TryToFire();
};
