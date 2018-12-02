// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BattleTank.h"
#include "AIController.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	virtual void Tick(float DeltaTime) override;
	APawn * GetPlayerTank() const;

private:

	// How close can the AI tank get to the player
	UPROPERTY(EditDefaultsOnly)
	float AcceptanceRadius = 3000;

};
