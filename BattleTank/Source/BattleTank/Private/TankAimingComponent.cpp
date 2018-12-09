// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (RoundsLeft <= 0)
	{
		FiringState = EFiringState::OutOfAmmo;
		RoundsLeft = 0;
	}
	else if ((FPlatformTime::Seconds() - LastFireTime) < ReloadTimeInSeconds)
	{
		FiringState = EFiringState::Reloading;
	}
	else if (IsBarrelMoving())
	{
		FiringState = EFiringState::Aiming;

	}
	else
	{
		FiringState = EFiringState::Locked;
	}
}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

int32 UTankAimingComponent::GetRoundsLeft() const
{
	return RoundsLeft;
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }

	FVector BarrelForward = Barrel->GetForwardVector();

	float Tolerance = 0.1f;
	return !BarrelForward.GetSafeNormal().Equals(AimDirection.GetSafeNormal(), Tolerance);
}

void UTankAimingComponent::Initialize(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel)) { return; }

	FString OurTankName = GetOwner()->GetName();
	FVector BarrelLocation = Barrel->GetComponentLocation();

	FVector OutLaunchVelocity(0);
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	// Calculate the OutLaunchVelocity
	bool bHaveAimSolution = UGameplayStatics::SuggestProjectileVelocity
	(
		this,                                          // const UObject * WorldContextObject,
		OutLaunchVelocity,                             // FVector & TossVelocity,
		StartLocation,                                 // FVector StartLocation,
		HitLocation,                                   // FVector EndLocation,
		LaunchSpeed,                                   // float TossSpeed,
		false,                                         // bool bHighArc,
		0,                                             // float CollisionRadius,
		0,                                             // float OverrideGravityZ,
		ESuggestProjVelocityTraceOption::DoNotTrace    // ESuggestProjVelocityTraceOption::Type TraceOption,
	);	                                               // const FCollisionResponseParams & ResponseParam,
			                                           // const TArray < AActor * > & ActorsToIgnore,
			                                           // bool bDrawDebug

	if (bHaveAimSolution)
	{
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		FString TankName = GetOwner()->GetName();

		DrawDebugLine(GetWorld(), StartLocation, HitLocation, FColor::Red, false, 0.0f, 0.0f, 4.0f);

		MoveBarrelTowards(AimDirection);
	}
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel)) { return; }
	if (!ensure(Turret)) { return; }

	// Work-out difference between current barrel rotation, and AimDirection
	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = AimDirection.Rotation();
	FRotator DeltaRotator = AimAsRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch);
	// Always yaw the shortest way
	if (FMath::Abs(DeltaRotator.Yaw) < 180)
	{
		Turret->Rotate(DeltaRotator.Yaw);
	}
	else // Avoid going the long way around
	{
		Turret->Rotate(-DeltaRotator.Yaw);
	}
}

void UTankAimingComponent::Fire()
{
	if (!ensure(Barrel)) { return; }

	if (FiringState == EFiringState::Locked ||
		FiringState == EFiringState::Aiming)
	{
		// Spawn a projectile at the socket location
		AProjectile * Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		// UE_LOG(LogTemp, Warning, TEXT("Projectile fired at launch speed %f"), LaunchSpeed);
		LastFireTime = FPlatformTime::Seconds();
		RoundsLeft--;
	}
}
