// Copyright 2024 DME Games


#include "DNM_ProjectileBase.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ADNM_ProjectileBase::ADNM_ProjectileBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	SetRootComponent(MeshComp);
	
	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));

	// Set default variables
	DamagePerBullet = 34;
}

// Called when the game starts or when spawned
void ADNM_ProjectileBase::BeginPlay()
{
	Super::BeginPlay();

	MeshComp->OnComponentHit.AddDynamic(this, &ADNM_ProjectileBase::ComponentHit);
}

// Called every frame
void ADNM_ProjectileBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADNM_ProjectileBase::ComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit %s"), *OtherActor->GetName());
}
