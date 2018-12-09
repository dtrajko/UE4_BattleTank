// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/TankAIController.h"
#include "TankAimingComponent.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Tank.h" // so we can implement OnDeath()


void ATankAIController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (!InPawn) { return; }
	ATank* PossesedTank = Cast<ATank>(InPawn);
	if (!ensure(PossesedTank)) { return; }
	PossesedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
}

void ATankAIController::OnPossessedTankDeath()
{
	if (!ensure(GetPawn())) { return; }
	GetPawn()->DetachFromControllerPendingDestroy();
}

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
	FVector AimHigher(0, 0, 80); // aim higher than the ground level
	TankAimingComponent->AimAt(PlayerTank->GetActorLocation() + AimHigher);

	// Fire if ready
	if (TankAimingComponent->GetFiringState() == EFiringState::Locked)
	{
		TankAimingComponent->Fire();
	}
}

APawn * ATankAIController::GetPlayerTank() const
{
	APawn * PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!ensure(PlayerPawn)) { return nullptr; }

	return PlayerPawn;
}
