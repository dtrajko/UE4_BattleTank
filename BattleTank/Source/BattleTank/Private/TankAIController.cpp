// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/TankAIController.h"
#include "TankAimingComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"


void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn * PlayerTank = GetPlayerTank();
	UTankAimingComponent * TankAimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();

	if (!ensure(PlayerTank)) { return; }
	if (!ensure(TankAimingComponent)) { return; }

	// TODO move towards the player
	MoveToActor(PlayerTank, AcceptanceRadius);

	// Aim towards the player
	FVector AimHigher(0, 0, 140); // aim higher than the ground level
	TankAimingComponent->AimAt(PlayerTank->GetActorLocation() + AimHigher);

	// Fire if ready
	TankAimingComponent->Fire();
}

APawn * ATankAIController::GetPlayerTank() const
{
	APawn * PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!ensure(PlayerPawn)) { return nullptr; }

	return PlayerPawn;
}
