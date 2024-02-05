// Copyright 2024 DME Games


#include "DNM_ProjectileBase.h"
#include "EnemyCharacterBase.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ADNM_ProjectileBase::ADNM_ProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(SceneComp);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(SceneComp);
	
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	
	// Set default variables
	DamagePerBullet = 34;
}

// Called when the game starts or when spawned
void ADNM_ProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(3.0f);
	MeshComp->OnComponentBeginOverlap.AddDynamic(this, &ADNM_ProjectileBase::ComponentBeginOverlap);
}

// Called every frame
void ADNM_ProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADNM_ProjectileBase::DestroyAfterDelay()
{
	GetWorld()->GetTimerManager().ClearTimer(DestroyTimer);
	Destroy();
}

void ADNM_ProjectileBase::ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (AEnemyCharacterBase* EnemyHit = Cast<AEnemyCharacterBase>(OtherActor))
	{
		EnemyHit->DealWithProjectile(this);
		GetWorld()->GetTimerManager().SetTimer(DestroyTimer, this, &ADNM_ProjectileBase::DestroyAfterDelay, 0.03333f, false);
	}
	
}
