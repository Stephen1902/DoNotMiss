// Copyright 2024 DME Games

#include "DNM_PlayerPawn.h"
#include "DNM_WeaponBase.h"
#include "DoNotMissGameModeBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ADNM_PlayerPawn::ADNM_PlayerPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Comp"));
	SetRootComponent(CapsuleComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Comp"));
	SkeletalMeshComponent->SetupAttachment(CapsuleComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Comp"));

	TimeBetweenFiring = 0.0f;
	TimeSinceLastFired = 0.0f;
	bGameIsPaused = false;
}

// Called when the game starts or when spawned
void ADNM_PlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	PlayerController->SetShowMouseCursor(true);

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

// Called every frame
void ADNM_PlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotatePlayer(DeltaTime);

	if (!bGameIsPaused)
	{
		TimeSinceLastFired += DeltaTime;
	}
}

// Called to bind functionality to input
void ADNM_PlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("TryToFire", IE_Pressed, this, &ADNM_PlayerPawn::TryToFire);
}

void ADNM_PlayerPawn::RotatePlayer(float DeltaTime)
{
	if (PlayerController)
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursorByChannel(TraceTypeQuery2, false, HitResult);
		const FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), HitResult.ImpactPoint);
		const FRotator MouseRotation = FRotator(0.f, LookAtRot.Yaw, 0.f);
		
		const FRotator NewActorRotation = UKismetMathLibrary::RInterpTo(GetActorRotation(), MouseRotation, DeltaTime, 2.0f );
		SetActorRotation(NewActorRotation);
	}
}

void ADNM_PlayerPawn::SpawnPlayerWeapon(TSubclassOf<ADNM_WeaponBase> WeaponToUse, FName SocketToUse)
{
	const FActorSpawnParameters SpawnParameters;
	PlayerWeapon = GetWorld()->SpawnActor<ADNM_WeaponBase>(WeaponToUse, GetActorLocation(), GetActorRotation(), SpawnParameters);
	const FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

	PlayerWeapon->AttachToComponent(SkeletalMeshComponent, TransformRules, SocketToUse);
	PlayerBullets = WeaponToUse.GetDefaultObject()->GetCurrentAmmo();
	TimeBetweenFiring = WeaponToUse.GetDefaultObject()->GetTimeBetweenFiring();
	TimeSinceLastFired = TimeBetweenFiring;
}

void ADNM_PlayerPawn::TryToFire()
{
	if (!bGameIsPaused && TimeSinceLastFired > TimeBetweenFiring)
	{
		if (PlayerWeapon->TryToFire())
		{
			TimeSinceLastFired = 0.f;
		}
	}
}
