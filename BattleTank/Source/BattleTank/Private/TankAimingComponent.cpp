   // Fill out your copyright notice in the Description page of Project Settings.

#include "Runtime/Engine/Classes/Engine/World.h"
#include "TankAimingComponent.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true; // TODO - Should this really tick?

	// ...
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel * BarrelToSet)
{
	Barrel = BarrelToSet;
}

void UTankAimingComponent::SetTurretReference(UStaticMeshComponent * TurretToSet)
{
	Turret = TurretToSet;
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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
		this,                                        // const UObject * WorldContextObject,
		OutLaunchVelocity,                           // FVector & TossVelocity,
		StartLocation,                               // FVector StartLocation,
		HitLocation,                                 // FVector EndLocation,
		LaunchSpeed,                                 // float TossSpeed,
		false,                                       // bool bHighArc,
		0,                                           // float CollisionRadius,
		0,                                           // float OverrideGravityZ,
		ESuggestProjVelocityTraceOption::DoNotTrace  // ESuggestProjVelocityTraceOption::Type TraceOption,
	);	                                             // const FCollisionResponseParams & ResponseParam,
			                                         // const TArray < AActor * > & ActorsToIgnore,
			                                         // bool bDrawDebug

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

		auto Time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("%f: Aim solution found."), Time)
	}
	else
	{
		auto Time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("%f: No aim solution found."), Time)
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

	Barrel->Elevate(5); // TODO - remove magic number (5 degrees per second)
}
