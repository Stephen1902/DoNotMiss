// Copyright 2024 DME Games


#include "EnemyCharacterBase.h"
#include "DNM_AIController.h"
#include "DNM_PlayerController.h"
#include "DNM_ProjectileBase.h"
#include "DNM_PlayerPawn.h"
#include "Kismet/GameplayStatics.h"

#define ProjectileTrace ECC_GameTraceChannel1 

// Sets default values
AEnemyCharacterBase::AEnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use custom trace channel for projectiles so the only thing that causes projectile to overlap is this
	GetMesh()->SetCollisionResponseToChannel(ProjectileTrace, ECR_Block);
	
	StartingHealth = 100.f;
	CurrentHealth = StartingHealth;
	DelayBeforeMoving = 0.2f;

	static ConstructorHelpers::FClassFinder<ADNM_AIController> FoundAIController(TEXT("/Game/Blueprints/Enemies/BP_AIController"));
	if (FoundAIController.Succeeded())
	{
		AutoPossessAI = EAutoPossessAI::Disabled;
		AIControllerClass = FoundAIController.Class;
	}
}

void AEnemyCharacterBase::DealWithProjectile(ADNM_ProjectileBase* ProjectileThatHit)
{
	if (ProjectileThatHit != nullptr)
	{
		// Add the projectile that hit the enemy to the array
		ProjectilesThatHit.Add(ProjectileThatHit);

		const int32 DamagedCaused = ProjectileThatHit->GetDamagePerBullet();
		if (DamagedCaused > 0)
		{
			CurrentHealth -= DamagedCaused;

			UE_LOG(LogTemp, Warning, TEXT("Current Health of %s is %f"), *GetName(), CurrentHealth);
			if (CurrentHealth <= 0)
			{
				EnemyHasDied();
			}
		}
	}
}

// Called when the game starts or when spawned
void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = StartingHealth;

	if (AIControllerClass)
	{
		const FActorSpawnParameters SpawnParameters;
		ADNM_AIController* PawnController = GetWorld()->SpawnActor<ADNM_AIController>(AIControllerClass, GetActorLocation(), GetActorRotation(), SpawnParameters);
		PawnController->Possess(this);
	}
	
}

// Called every frame
void AEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacterBase::EnemyHasDied()
{
	// Get the player character
	if (ADNM_PlayerController* PlayerController = Cast<ADNM_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PlayerController->ReturnPlayerBullet(ProjectilesThatHit.Num());
		// TODO Change this so it returns the actual type of bullet, not just a number of bullets for gun switching later
		for (int32 i = 0; i < ProjectilesThatHit.Num(); ++i)
		{
			
		}
	}

	GetMesh()->SetSimulatePhysics(true);
	SetLifeSpan(2.0f);
}