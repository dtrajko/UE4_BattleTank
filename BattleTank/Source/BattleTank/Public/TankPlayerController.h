// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"


class UTankAimingComponent;

/**
 * Responsible for helping the player aim
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
		void FoundAimingComponent(UTankAimingComponent * TankAimCompRef);

private:

	// Start the tank moving the barrel so that a shot
	// would hit where the crosshair intersects the world
	void AimTowardsCrosshair();

	// Return an OUT parameter, true if hit landscape
	bool GetSightRayHitLocation(OUT FVector &HitLocation) const;

	bool GetLookDirection(FVector2D &ScreenLocation, FVector &LookDirection) const;

	bool GetLookVectorHitLocation(FVector &LookDirection, OUT FVector &HitLocation) const;

private:

	UPROPERTY(EditDefaultsOnly)
	float CrossHairLocationX = 0.5f;

	UPROPERTY(EditDefaultsOnly)
	float CrossHairLocationY = 0.3333f;

	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000.0f; // 10km = 10.000m = 1.000.000cm

};
