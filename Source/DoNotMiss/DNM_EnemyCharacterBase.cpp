// Copyright 2024 DME Games


#include "DNM_EnemyCharacterBase.h"
#include "DNM_AIController.h"
#include "DNM_PlayerBarrier.h"
#include "DNM_PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Kismet/GameplayStatics.h"

#define ProjectileTrace ECC_GameTraceChannel1 

// Sets default values
ADNM_EnemyCharacterBase::ADNM_EnemyCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HairMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Hair Mesh"));
	HairMesh->SetupAttachment(GetMesh());
	HairMesh->SetMasterPoseComponent(GetMesh());

	ShirtMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shirt Mesh"));
	ShirtMesh->SetupAttachment(GetMesh());
	ShirtMesh->SetMasterPoseComponent(GetMesh());

	TrousersMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Trousers Mesh"));
	TrousersMesh->SetupAttachment(GetMesh());
	TrousersMesh->SetMasterPoseComponent(GetMesh());

	ShoesMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Shoes Mesh"));
	ShoesMesh->SetupAttachment(GetMesh());
	ShoesMesh->SetMasterPoseComponent(GetMesh());
	
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

void ADNM_EnemyCharacterBase::DealWithProjectile(const float DamageCaused, const int32 DamageDivisor)
{
	if (DamageCaused > 0 && CurrentHealth > 0.f)
	{
		ProjectilesThatHit += (1.f / DamageDivisor);
		CurrentHealth -= DamageCaused;
		
		if (CurrentHealth <= 0.f)
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

	SetCharacterParams();
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
		PlayerControllerRef->AlterPlayerBullet(ProjectilesThatHit);
		PlayerControllerRef->EnemyHasDied(this);
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

void ADNM_EnemyCharacterBase::SetCharacterParams()
{
	const int32 RandomCharacterSelect = FMath::RandRange(1, 3);
	const bool CharHasClothes = FMath::FRandRange(1.f, 100.f) <= 50.f;

	switch (RandomCharacterSelect)
	{
		// Options for the Hank Character
		case 1:
		default:
			if (CharHasClothes)
			{
				const int32 SelectClothedMeshHank = FMath::RandRange(0, HankBodyMeshOptions.Num() - 1);
				GetMesh()->SetSkeletalMesh(HankBodyMeshOptions[SelectClothedMeshHank]);
			}
			else
			{
				GetMesh()->SetSkeletalMesh(HankNoClothesOptions[0]);
				if (FMath::FRandRange(1.f, 100.f) < 50.f)
				{
					HairMesh->SetSkeletalMesh(HankNoClothesOptions[1]);
					HairMesh->SetForcedLOD(3);
				}
				if (FMath::FRandRange(1.f, 100.f) < 50.f)
				{
					ShirtMesh->SetSkeletalMesh(HankNoClothesOptions[2]);
					ShirtMesh->SetForcedLOD(3);
				}
				if (FMath::FRandRange(1.f, 100.f) < 50.f)
				{
					TrousersMesh->SetSkeletalMesh(HankNoClothesOptions[3]);
					TrousersMesh->SetForcedLOD(3);
				}
				if (FMath::FRandRange(1.f, 100.f) < 50.f)
				{
					ShoesMesh->SetSkeletalMesh(HankNoClothesOptions[4]);
					ShoesMesh->SetForcedLOD(3);
				}
				
			}
			break;
		// Options for Maria Character 
		case 2:
			if (CharHasClothes)
			{
				const int32 SelectClothedMeshMaria = FMath::RandRange(0, MariaBodyMeshOptions.Num() - 1);
				GetMesh()->SetSkeletalMesh(MariaBodyMeshOptions[SelectClothedMeshMaria]);
			}
			else
			{
				GetMesh()->SetSkeletalMesh(MariaNoClothesOptions[0]);
				if (FMath::FRandRange(1.f, 100.f) < 50.f)
				{
					HairMesh->SetSkeletalMesh(MariaNoClothesOptions[1]);
					HairMesh->SetForcedLOD(3);
				}
				if (FMath::FRandRange(1.f, 100.f) < 50.f)
				{
					ShirtMesh->SetSkeletalMesh(MariaNoClothesOptions[2]);
					ShirtMesh->SetForcedLOD(3);
				}
				if (FMath::FRandRange(1.f, 100.f) < 50.f)
				{
					TrousersMesh->SetSkeletalMesh(MariaNoClothesOptions[3]);
					TrousersMesh->SetForcedLOD(3);
				}
				if (FMath::FRandRange(1.f, 100.f) < 50.f)
				{
					ShoesMesh->SetSkeletalMesh(MariaNoClothesOptions[4]);
					ShoesMesh->SetForcedLOD(3);
				}
		}
			break;

		// Options for Mike Character
		case 3:
			if (CharHasClothes)
			{
				const int32 SelectClothedMeshMike = FMath::RandRange(0, MikeBodyMeshOptions.Num() - 1);
				GetMesh()->SetSkeletalMesh(MikeBodyMeshOptions[SelectClothedMeshMike]);
			}
			else
			{
				GetMesh()->SetSkeletalMesh(MikeNoClothesOptions[0]);
				if (FMath::FRandRange(1.f, 100.f) < 50.f)
				{
					HairMesh->SetSkeletalMesh(MikeNoClothesOptions[1]);
					HairMesh->SetForcedLOD(3);
				}
				if (FMath::FRandRange(1.f, 100.f) < 50.f)
				{
					ShirtMesh->SetSkeletalMesh(MikeNoClothesOptions[2]);
					ShirtMesh->SetForcedLOD(3);
				}
				if (FMath::FRandRange(1.f, 100.f) < 50.f)
				{
					TrousersMesh->SetSkeletalMesh(MikeNoClothesOptions[3]);
					TrousersMesh->SetForcedLOD(3);
				}
				if (FMath::FRandRange(1.f, 100.f) < 50.f)
				{
					ShoesMesh->SetSkeletalMesh(MikeNoClothesOptions[4]);
					ShoesMesh->SetForcedLOD(3);
				}
			}
			break;
	}

	GetMesh()->SetForcedLOD(3);
}
