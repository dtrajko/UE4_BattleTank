// Fill out your copyright notice in the Description page of Project Settings.

#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "TankAimingComponent.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel * BarrelToSet)
{
	if (!BarrelToSet) return;
	Barrel = BarrelToSet;
}

void UTankAimingComponent::SetTurretReference(UTankTurret * TurretToSet)
{
	if (!TurretToSet) return;
	Turret = TurretToSet;
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed)
{
	if (!Barrel) { return; }

	FString OurTankName = GetOwner()->GetName();
	FVector BarrelLocation = Barrel->GetComponentLocation();

	FVector OutLaunchVelocity(0);
	FVector StartLocation = Barrel->GetSocketLocation(FName("TubeEnd"));

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

	// UE_LOG(LogTemp, Warning, TEXT("bHaveAimSolution: %s, HitLocation: %s"), (bHaveAimSolution ? "TRUE" : "FALSE"), *HitLocation.ToString())

	if (bHaveAimSolution)
	{
		FVector AimDirection = OutLaunchVelocity.GetSafeNormal();
		FString TankName = GetOwner()->GetName();

		// UE_LOG(LogTemp, Warning, TEXT("SuggestProjectileVelocity OutLaunchVelocity: %s, LaunchSpeed: %f, StartLocation: %s, HitLocation: %s, AimDirection: %s"),
		// 	*OutLaunchVelocity.ToString(), LaunchSpeed, *StartLocation.ToString(), *HitLocation.ToString(), *AimDirection.ToString())
		// UE_LOG(LogTemp, Warning, TEXT("%s aiming at %s from %s"), *OurTankName, *HitLocation.ToString(), *BarrelLocation.ToString())

		// UE_LOG(LogTemp, Warning, TEXT("Tank %s aiming at %s"), *TankName, *AimDirection.ToString())
		DrawDebugLine(GetWorld(), StartLocation, HitLocation, FColor::Red, false, 0.0f, 0.0f, 2.0f);

		MoveBarrelTowards(AimDirection);

		// auto Time = GetWorld()->GetTimeSeconds();
		// UE_LOG(LogTemp, Warning, TEXT("%f: [%s] Aim solution found."), Time, *GetOwner()->GetName())
	}
	else
	{
		// auto Time = GetWorld()->GetTimeSeconds();
		// UE_LOG(LogTemp, Warning, TEXT("%f: [%s] No aim solution found."), Time, *GetOwner()->GetName())
	}
	// If solution not found - do nothing
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	// Work-out difference between current barrel rotation, and AimDirection
	FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
	FRotator AimAsRotator = AimDirection.Rotation();
	FRotator DeltaRotator = AimAsRotator - BarrelRotator;

	// UE_LOG(LogTemp, Warning, TEXT("BarrelRotator: %s, AimAsRotator: %s, DeltaRotator: %s"),
	// 	*BarrelRotator.ToString(), *AimAsRotator.ToString(), *DeltaRotator.ToString())

	Barrel->Elevate(DeltaRotator.Pitch);
	Turret->Rotate(DeltaRotator.Yaw);
}
