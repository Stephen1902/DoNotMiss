// Copyright 2024 DME Games

#include "DNM_PlayerPawn.h"

#include "DNM_PlayerController.h"
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

	bGameIsRunning = true;
}

// Called when the game starts or when spawned
void ADNM_PlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<ADNM_PlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

// Called every frame
void ADNM_PlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bGameIsRunning)
	{
		RotatePlayer(DeltaTime);
	}
}

// Called to bind functionality to input
void ADNM_PlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
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



