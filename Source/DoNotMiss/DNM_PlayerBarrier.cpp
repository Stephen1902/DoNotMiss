// Copyright 2024 DME Games


#include "DNM_PlayerBarrier.h"
#include "DNM_EnemyCharacterBase.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values
ADNM_PlayerBarrier::ADNM_PlayerBarrier()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root Comp"));
	SetRootComponent(RootComp);

	CollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Comp"));
	CollisionComp->SetupAttachment(RootComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh Comp"));
	MeshComp->SetupAttachment(CollisionComp);

	BarrierWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget"));
	BarrierWidget->SetupAttachment(RootComp);
	BarrierWidget->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
	
}

void ADNM_PlayerBarrier::EnemyHasTakenAmmo(const int32 NumAmmoTaken)
{
	const FText TextToSend = FText::FromString(FString::FromInt(NumAmmoTaken));
	PlayWidgetAnimation(TextToSend);
	UE_LOG(LogTemp, Warning, TEXT("Player Barrier has given ammo"));
}

// Called when the game starts or when spawned
void ADNM_PlayerBarrier::BeginPlay()
{
	Super::BeginPlay();
	
}
