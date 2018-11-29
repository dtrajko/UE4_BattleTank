// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	// -1 is max downward movement, +1 is max upward movement
	void Elevate(float RelativeSpeed);

private:

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float MaxDegreesPerSecond = 10; // Sensible default

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float MinElevationDegrees = 0; // TODO - Set MinElevationDegrees

	UPROPERTY(EditDefaultsOnly, Category = "Setup")
		float MaxElevationDegrees = 45; // TODO - Set MaxElevationDegrees
};
