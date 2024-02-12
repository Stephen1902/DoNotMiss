// Copyright 2024 DME Games

#include "DNM_PlayerController.h"
#include "DNM_WeaponBase.h"
#include "DNM_GameStateBase.h"
#include "DNM_PlayerPawn.h"
#include "DNM_PlayerWidget.h"
#include "DNM_PauseWidget.h"
#include "Kismet/GameplayStatics.h"

ADNM_PlayerController::ADNM_PlayerController()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TimeBetweenFiring = 0.0f;
	TimeSinceLastFired = 0.0f;
	PlayerBullets = 0;
	EnemiesKilled = 0;
	bGameIsRunning = false;
}

void ADNM_PlayerController::GameHasStarted()
{
	bGameIsRunning = true;
	
	if (GameStateRef && ControlledPawn)
	{
		ControlledPawn->SetGameIsRunning(true);
		GameStateRef->SetGameIsRunning(true);
	}
}

void ADNM_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetShowMouseCursor(true);
	SetPlayerWeapon();	
	SetGameStateRef();
	CreatePlayerWidgets();
}

void ADNM_PlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bGameIsRunning)
	{
		TimeSinceLastFired += DeltaSeconds;
	}
}

void ADNM_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("TryToFire", IE_Pressed, this, &ADNM_PlayerController::TryToFire);
	InputComponent->BindAction("TogglePause", IE_Pressed, this, &ADNM_PlayerController::TogglePauseWidget);
}

void ADNM_PlayerController::SetPlayerWeapon()
{
	const FString PassedInOption = UGameplayStatics::GetGameMode(GetWorld())->OptionsString;;

	if (PassedInOption.Contains("pistol") && PistolBP != nullptr)
	{
		SpawnPlayerWeapon(PistolBP, FName("PistolSocket"));
	}
	else if (PassedInOption.Contains("shotgun") && ShotgunBP != nullptr)
	{
		SpawnPlayerWeapon(ShotgunBP, FName("WeaponSocket"));
	}
	else if (PassedInOption.Contains("rifle") && RifleBP != nullptr)
	{
		SpawnPlayerWeapon(RifleBP, FName("WeaponSocket"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("The option the player selected (%s) doesn't contain a valid Blueprint"), *PassedInOption);
	}
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

void ADNM_PlayerController::CreatePlayerWidgets()
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

	if (PauseWidget)
	{
		PauseWidgetRef = CreateWidget<UDNM_PauseWidget>(this, PauseWidget);
		PauseWidgetRef->SetPlayerControllerRef(this);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Player Controller does not have a valid pause widget set"));
	}

}

void ADNM_PlayerController::TogglePauseWidget()
{
	if (PlayerWidgetRef && PauseWidgetRef)
	{
		if (!PauseWidgetRef->IsInViewport())
		{
			bGameIsRunning = false;
	
			// Tell the game state and Player Pawn the game status has changed
			if (GameStateRef && ControlledPawn)
			{
				ControlledPawn->SetGameIsRunning(bGameIsRunning);
				GameStateRef->SetGameIsRunning(bGameIsRunning);
			}

			// Remove the player widget
			PlayerWidgetRef->RemoveFromParent();
			// Add the Pause Widget to the viewport
			PauseWidgetRef->AddToViewport();
		}
		else
		{
			// Remove the Pause Widget
			PauseWidgetRef->RemoveFromParent();
			// Add the Player Widget to the viewport
			PlayerWidgetRef->AddToViewport();
		}
	}
}

void ADNM_PlayerController::SpawnPlayerWeapon(TSubclassOf<ADNM_WeaponBase> WeaponToUse, FName SocketToUse)
{
	ControlledPawn = Cast<ADNM_PlayerPawn>( UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	if (!ControlledPawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to get a controlled pawn in Player Controller"));
		return;
	}
	
	const FActorSpawnParameters SpawnParameters;
	PlayerWeapon = GetWorld()->SpawnActor<ADNM_WeaponBase>(WeaponToUse, ControlledPawn->GetActorLocation(), ControlledPawn->GetActorRotation(), SpawnParameters);
	const FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

	PlayerWeapon->AttachToComponent(ControlledPawn->GetSkeletalMeshComp(), TransformRules, SocketToUse);
	PlayerBullets = WeaponToUse.GetDefaultObject()->GetCurrentAmmo();
	TimeBetweenFiring = WeaponToUse.GetDefaultObject()->GetTimeBetweenFiring();
	TimeSinceLastFired = TimeBetweenFiring;
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

void ADNM_PlayerController::ReturnPlayerBullet(const int32 BulletNumber)
{
	PlayerBullets += BulletNumber;
	if (PlayerWidgetRef)
	{
		PlayerWidgetRef->UpdateBulletCount(FText::FromString(FString::FromInt(PlayerBullets)));
	}
}

void ADNM_PlayerController::EnemyHasDied()
{
	EnemiesKilled += 1;
	if (PlayerWidgetRef)
	{
		PlayerWidgetRef->UpdateEnemyKilledCount(FText::FromString(FString::FromInt(EnemiesKilled)));
	}
	
	if (GameStateRef)
	{
		GameStateRef->EnemyHasDied();
	}
}


void ADNM_PlayerController::TryToFire()
{
	if (bGameIsRunning && PlayerWidgetRef && TimeSinceLastFired > TimeBetweenFiring)
	{
		if (PlayerWeapon->TryToFire())
		{
			PlayerBullets -= PlayerWeapon->GetBulletsPerFire();
			TimeSinceLastFired = 0.f;
			PlayerWidgetRef->UpdateBulletCount(FText::FromString(FString::FromInt(PlayerBullets)));
		}
	}
}
