// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"


void UTankTrack::SetThrottle(float Throttle)
{
	FString Name = GetName();
	// UE_LOG(LogTemp, Warning, TEXT("%s throttle %f"), *Name, Throttle);

	// Clamp actual throttle value so player can't overdrive
	FVector ForceApplied = GetForwardVector() * Throttle * TrackMaxDrivingForce;
	FVector ForceLocation = GetComponentLocation();
	UPrimitiveComponent * TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation, NAME_None);
}
