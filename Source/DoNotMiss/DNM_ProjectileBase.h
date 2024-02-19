// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DNM_ProjectileBase.generated.h"

UCLASS()
class DONOTMISS_API ADNM_ProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADNM_ProjectileBase();

	float GetDamagePerBullet() const { return DamagePerBullet * DamageDivision; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	USceneComponent* SceneComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	UStaticMeshComponent* MeshComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	class UProjectileMovementComponent* ProjectileMovementComp;

	// The amount of damage each piece of ammo causes
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	float DamagePerBullet;

	// Amount the damage needs to be divided by ie, if bullet is a shotgun shell
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up", meta=(ClampMin=1.0f))
	float DamageDivision;

	UFUNCTION()
	void ComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	FTimerHandle DestroyTimer;
	void DestroyAfterDelay();
};

