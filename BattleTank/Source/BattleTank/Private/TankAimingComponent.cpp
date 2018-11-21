   // Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::SetBarrelReference(UStaticMeshComponent * BarrelToSet)
{
	Barrel = BarrelToSet;
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
	// UE_LOG(LogTemp, Warning, TEXT("%s aiming at %s from %s"), *OurTankName, *HitLocation.ToString(), *BarrelLocation.ToString())
	DrawDebugLine(GetWorld(), BarrelLocation, HitLocation, FColor::Red, false, 0.0f, 0.0f, 2.0f);

	FVector OutLaunchVelocity(0);
	FVector StartLocation = Barrel->GetSocketLocation(FName("TubeEnd"));

	// Calculate the OutLaunchVelocity
	bool bResult = UGameplayStatics::SuggestProjectileVelocity
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

	if (bResult)
	{
		FVector AimDirection = OutLaunchVelocity.GetSafeNormal();
		FString TankName = GetOwner()->GetName();

		// UE_LOG(LogTemp, Warning, TEXT("SuggestProjectileVelocity OutLaunchVelocity: %s, LaunchSpeed: %f, StartLocation: %s, HitLocation: %s, AimDirection: %s"),
		// 	*OutLaunchVelocity.ToString(), LaunchSpeed, *StartLocation.ToString(), *HitLocation.ToString(), *AimDirection.ToString())
		UE_LOG(LogTemp, Warning, TEXT("Tank %s aiming at %s"), *TankName, *AimDirection.ToString())
	}
	// If solution not found - do nothing
}
