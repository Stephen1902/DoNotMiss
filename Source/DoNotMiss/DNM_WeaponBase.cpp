// Copyright 2024 DME Games


#include "DNM_WeaponBase.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ADNM_WeaponBase::ADNM_WeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	SetRootComponent(SceneComp);

	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(SceneComp);
	
	// Set default variables
	WeaponType = EWeaponType::WT_Pistol;
	StartingAmmo = 100;
	CurrentAmmo = StartingAmmo;
	bBurstFire = false;
	NumberPerBurst = 0;
	TimeBetweenFiring = 0.5f;
}

// Called when the game starts or when spawned
void ADNM_WeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADNM_WeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ADNM_WeaponBase::TryToFire()
{
	Fire();
	return true;
}

void ADNM_WeaponBase::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Fire!"));
}

