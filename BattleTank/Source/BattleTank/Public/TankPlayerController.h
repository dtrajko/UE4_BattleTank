// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleTank.h"
#include "Tank.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:

	ATank * GetControlledTank() const;

	// Start the tank moving the barrel so that a shot
	// would hit where the crosshair intersects the world
	void AimTowardsCrosshair();

	// Return an OUT parameter, true if hit landscape
	bool GetSightRayHitLocation(OUT FVector & HitLocation) const;

private:

	UPROPERTY(EditAnywhere)
	float CrossHairLocationX = 0.5f;

	UPROPERTY(EditAnywhere)
	float CrossHairLocationY = 0.3333f;
	
};
