// Copyright 2024 DME Games


#include "DNM_WeaponBase.h"

#include "DNM_ProjectileBase.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

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
	TimeBetweenFiring = 0.5f;
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
	UE_LOG(LogTemp, Warning, TEXT("Fire!"));
	const FActorSpawnParameters SpawnParameters;
	const FTransform SocketTransform = MeshComp->GetSocketTransform(FName("FiringSocket"));
	ADNM_ProjectileBase* SpawnedBullet = GetWorld()->SpawnActor<ADNM_ProjectileBase>(BulletToSpawn, SocketTransform, SpawnParameters);

	//UGameplayStatics::SetGamePaused(GetWorld(), true);
	
	const FString StringToDisplay = "X: " + FString::SanitizeFloat(SocketTransform.GetLocation().X) + " Y: " + FString::SanitizeFloat(SocketTransform.GetLocation().Y) + " Z: " + FString::SanitizeFloat(SocketTransform.GetLocation().Z);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, StringToDisplay);

	// const FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);
	// SpawnedBullet->AttachToComponent(MeshComp, TransformRules, FName("FiringSocket"));

}

