// Fill out your copyright notice in the Description page of Project Settings.

#include "../Public/TankAIController.h"
#include "Tank.h"


void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	auto ControlledTank = GetControlledTank();
	if (ensure(ControlledTank))
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController posessing %s"), *(ControlledTank->GetName()));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController not possessing a tank."));
	}
	auto PlayerTank = GetPlayerTank();
	if (ensure(PlayerTank))
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController of the tank %s found the player tank %s"), *(ControlledTank->GetName()), *(PlayerTank->GetName()));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AIController can't find the player tank."));
	}
}

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	auto ControlledTank = Cast<ATank>(GetPawn());

	if (ensure(PlayerTank))
	{
		// TODO move towards the player
		MoveToActor(PlayerTank, AcceptanceRadius);

		// Aim towards the player
		FVector AimHigher(0, 0, 140); // aim higher than the ground level
		ControlledTank->AimAt(PlayerTank->GetActorLocation() + AimHigher);

		// Fire if ready
		ControlledTank->Fire();
	}
}

ATank * ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

ATank * ATankAIController::GetPlayerTank() const
{
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!ensure(PlayerPawn)) { return nullptr; }

	return Cast<ATank>(PlayerPawn);
}
