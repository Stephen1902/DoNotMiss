// Copyright 2024 DME Games


#include "DNM_WeaponBase.h"

#include "DNM_ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

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
	NumberPerBurst = 1;
	NumberFiredBurst = 0;
	TimeBetweenFiring = 0.5f;
	TimeBetweenBurstFire = 0.2f;
}

int32 ADNM_WeaponBase::GetBulletsPerFire() const
{
	if (bBurstFire)
	{
		return NumberPerBurst;
	}

	return 1;
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
	if (BulletToSpawn != nullptr  && CurrentAmmo > 0)
	{
		Fire();
		return true;
	}
	 return false;
}

void ADNM_WeaponBase::Fire()
{
	const FActorSpawnParameters SpawnParameters;
	FTransform SocketTransform = MeshComp->GetSocketTransform(FName("FiringSocket"));
	if (WeaponType == EWeaponType::WT_Shotgun)
	{
		for (int32 i = 0; i < 12; ++i)
		{
			FVector SocketLocation = SocketTransform.GetLocation();
			SocketLocation.X = SocketLocation.X + FMath::FRandRange(-25.f, 25.f);
			SocketLocation.Y = SocketLocation.Y + FMath::FRandRange(-25.f, 25.f);
			SocketLocation.Z = SocketLocation.Z + FMath::FRandRange(-10.f, 10.f);
			ADNM_ProjectileBase* SpawnedBullet = GetWorld()->SpawnActor<ADNM_ProjectileBase>(BulletToSpawn, SocketLocation, SocketTransform.GetRotation().Rotator(), SpawnParameters);
			SpawnedBullet->SetDamageDivision(12);

			if (EjectShellToSpawn)
			{
				FTransform ShellEjectTransform = MeshComp->GetSocketTransform(FName("b_gun_shelleject"));
				GetWorld()->SpawnActor<ADNM_ProjectileBase>(EjectShellToSpawn, ShellEjectTransform, SpawnParameters);
			}
		}
	}
	else
	{
		ADNM_ProjectileBase* SpawnedBullet = GetWorld()->SpawnActor<ADNM_ProjectileBase>(BulletToSpawn, SocketTransform, SpawnParameters);
	}

	if (SoundOnFire)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), SoundOnFire);
	}

	if (FireParticle)
	{
		FTransform GunMuzzleTransform = MeshComp->GetSocketTransform(FName("MuzzleFlashSocket"));
		GunMuzzleTransform.SetScale3D(FVector(0.25f));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireParticle, GunMuzzleTransform);		
	}

	if (bBurstFire && !GetWorld()->GetTimerManager().IsTimerActive(BurstFireTimer))
	{
		NumberFiredBurst = NumberPerBurst - 1;
		GetWorld()->GetTimerManager().SetTimer(BurstFireTimer, this, &ADNM_WeaponBase::FireOnTimer,TimeBetweenBurstFire, true);
	}
}

void ADNM_WeaponBase::FireOnTimer()
{
	if (NumberFiredBurst > 0)
	{
		NumberFiredBurst -= 1;
		Fire();
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstFireTimer);
	}
}