// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "TankAimingComponent.generated.h"

// Forward Declarations
class UTankBarrel;
class UTankTurret;
class AProjectile;

// Enum for aiming state
UENUM()
enum class EFiringState : uint8
{
	Reloading,
	Aiming,
	Locked,
	OutOfAmmo
};

// Holds barrel's properties and Elevate() method 
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BATTLETANK_API UTankAimingComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Setup")
		void Initialize(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet);

	void AimAt(FVector HitLocation);

	UFUNCTION(BlueprintCallable, Category = "Firing")
		void Fire();

	EFiringState GetFiringState() const;

	UFUNCTION(BlueprintCallable, Category = "Firing")
		int32 GetRoundsLeft() const;

protected:

	UPROPERTY(BlueprintReadOnly, Category = "State")
		EFiringState FiringState = EFiringState::Reloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setup")
		TSubclassOf<AProjectile> ProjectileBlueprint = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
		float LaunchSpeed = 8000.0f; // Sensible starting value of 1000 m/s

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
		float ReloadTimeInSeconds = 4.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Firing")
		int32 RoundsLeft = 60;

private:

	UTankBarrel * Barrel = nullptr;
	UTankTurret * Turret = nullptr;

	// Sets default values for this component's properties
	UTankAimingComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) override;

	bool IsBarrelMoving();

	void MoveBarrelTowards(FVector AimDirection);

	// Called when the game starts
	virtual void BeginPlay() override;

	double LastFireTime = 0;

	FVector AimDirection;

};
