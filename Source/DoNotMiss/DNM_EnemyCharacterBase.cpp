// Copyright 2024 DME Games


#include "DNM_EnemyCharacterBase.h"
#include "DNM_AIController.h"
#include "DNM_PlayerBarrier.h"
#include "DNM_PlayerController.h"
#include "DNM_ProjectileBase.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

#define ProjectileTrace ECC_GameTraceChannel1 

// Sets default values
ADNM_EnemyCharacterBase::ADNM_EnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyHealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health Component"));
	EnemyHealthWidgetComp->SetupAttachment(GetCapsuleComponent());
	EnemyHealthWidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 110.f));
	EnemyHealthWidgetComp->SetWidgetSpace(EWidgetSpace::Screen);
	EnemyHealthWidgetComp->SetVisibility(false);
	
	HealthBarColour = FLinearColor(1.0f, 0.f, 0.f, 1.f);

	// Use custom trace channel for projectiles so the only thing that causes projectile to overlap is this
	GetMesh()->SetCollisionResponseToChannel(ProjectileTrace, ECR_Block);
	
	StartingHealth = 100.f;
	CurrentHealth = StartingHealth;
	DelayBeforeMoving = 0.2f;
	AmmoTakenPerHit = 3;
	DelayBetweenAmmoTake = 2.0f;

	static ConstructorHelpers::FClassFinder<ADNM_AIController> FoundAIController(TEXT("/Game/Blueprints/Enemies/BP_AIController"));
	if (FoundAIController.Succeeded())
	{
		AutoPossessAI = EAutoPossessAI::Disabled;
		AIControllerClass = FoundAIController.Class;
	}
}

void ADNM_EnemyCharacterBase::DealWithProjectile(ADNM_ProjectileBase* ProjectileThatHit, const float DamageCaused)
{
	if (ProjectileThatHit != nullptr)
	{
		/*
		// Check if the projectile already exists in the TMap
		if (ProjectilesThatHit.Contains(ProjectileThatHit))
		{
			const float CurrentDamage = ProjectilesThatHit[ProjectileThatHit];
			ProjectilesThatHit.Emplace(ProjectileThatHit, CurrentDamage + DamageCaused);
			const float NewDamage = ProjectilesThatHit[ProjectileThatHit];
			UE_LOG(LogTemp, Warning, TEXT("Damage Caused now: %s"), *FString::SanitizeFloat(NewDamage));
		}
		
		else*/
		{
			// Add the projectile that hit the enemy to the array
			ProjectilesThatHit.Add(ProjectileThatHit); //, DamageCaused);
		}
		
		//const float DamagedCaused = ProjectileThatHit->GetDamagePerBullet();
		if (DamageCaused > 0)
		{
			CurrentHealth -= DamageCaused;
			
			if (CurrentHealth <= 0)
			{
				if (EnemyHealthWidgetComp)
				{
					EnemyHealthWidgetComp->SetVisibility(false);
				}
				EnemyHasDied();
			}
			else
			{
				if (EnemyHealthWidgetComp)
				{
					EnemyHealthWidgetComp->SetVisibility(true);
					const float HealthToSend = 1.0f - ((StartingHealth - CurrentHealth) / StartingHealth);
					OnHealthChanged(HealthToSend);
					UE_LOG(LogTemp, Warning, TEXT("Health Percent is %s"), *FString::SanitizeFloat(HealthToSend));
				}
			}
		}
	}
}

// Called when the game starts or when spawned
void ADNM_EnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = StartingHealth;

	if (AIControllerClass)
	{
		const FActorSpawnParameters SpawnParameters;
		ADNM_AIController* PawnController = GetWorld()->SpawnActor<ADNM_AIController>(AIControllerClass, GetActorLocation(), GetActorRotation(), SpawnParameters);
		PawnController->Possess(this);
	}

	PlayerControllerRef  = Cast<ADNM_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (PlayerControllerRef)
	{
		PlayerControllerRef->OnGameRunningChanged.AddDynamic(this, &ADNM_EnemyCharacterBase::SetGameIsRunning);
	}
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ADNM_EnemyCharacterBase::ComponentBeginOverlap);
}

// Called every frame
void ADNM_EnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADNM_EnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADNM_EnemyCharacterBase::EnemyHasDied()
{
	// Get the player character
	if (PlayerControllerRef)
	{
		PlayerControllerRef->AlterPlayerBullet(ProjectilesThatHit.Num());
		// TODO Change this so it returns the actual type of bullet, not just a number of bullets for gun switching later
		for (int32 i = 0; i < ProjectilesThatHit.Num(); ++i)
		{
				
		}
		
		PlayerControllerRef->EnemyHasDied();
	}

	// If the timer is running for this character to take some ammo, clear it before destroying
	if (GetWorld()->GetTimerManager().IsTimerActive(TakeAmmoTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(TakeAmmoTimer);
	}
	
	GetMesh()->SetSimulatePhysics(true);
	SetLifeSpan(2.0f);
}

void ADNM_EnemyCharacterBase::TakeEnemyAmmo()
{
	if (AmmoTakenPerHit > 0 && PlayerBarrierRef && PlayerControllerRef)
	{
		PlayerBarrierRef->EnemyHasTakenAmmo(AmmoTakenPerHit);
		PlayerControllerRef->AlterPlayerBullet(AmmoTakenPerHit * -1);
	}
}

void ADNM_EnemyCharacterBase::SetGameIsRunning(const bool GameRunningIn)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(TakeAmmoTimer))
	{
		if (!GameRunningIn)
		{
			GetWorld()->GetTimerManager().PauseTimer(TakeAmmoTimer);
		}
		else
		{
			GetWorld()->GetTimerManager().UnPauseTimer(TakeAmmoTimer);
		}
	}

}

void ADNM_EnemyCharacterBase::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Only run this code if have not already hit a barrier
	if (PlayerBarrierRef == nullptr)
	{
		if (ADNM_PlayerBarrier* BarrierHit = Cast<ADNM_PlayerBarrier>(OtherActor))
		{
			PlayerBarrierRef = BarrierHit;
			GetWorld()->GetTimerManager().SetTimer(TakeAmmoTimer, this, &ADNM_EnemyCharacterBase::TakeEnemyAmmo, DelayBetweenAmmoTake, true);
		}
	}
}
