// Copyright 2024 DME Games

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DNM_WeaponBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	WT_Pistol		UMETA(DisplayName="Pistol"),
	WT_Shotgun		UMETA(DisplayName="Shotgun"),
	WT_MachineGun	UMETA(DisplayName="Machine Gun")
};

USTRUCT(BlueprintType) struct FReloadStruct
{
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Structs|Weapon Reload")
	bool bWeaponReloads;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Structs|Weapon Reload")
	int32 MaxInClip;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Structs|Weapon Reload")
	int32 CurrentInClip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Structs|Weapon Reload")
	float ReloadTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Structs|Weapon Reload")
	bool bAutoReload;

	FReloadStruct()
	{
		bWeaponReloads = false;
		MaxInClip = 6;
		CurrentInClip = MaxInClip;
		ReloadTime = 1.0f;
		bAutoReload = true;
	}
};

UCLASS()
class DONOTMISS_API ADNM_WeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADNM_WeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	USceneComponent* SceneComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	USkeletalMeshComponent* MeshComp;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	int32 StartingAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	FReloadStruct ReloadStruct;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	bool bBurstFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up", meta=(EditCondition="bBurstFire"))
	int32 NumberPerBurst;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Set Up")
	TSubclassOf<class ADNM_ProjectileBase> BulletToSpawn;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	int32 CurrentAmmo;
};
