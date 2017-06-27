// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"


void UTankTrack::Accelerate(float Throttle)
{
	Throttle = FMath::Clamp<float>(Throttle, -1, 1);
	auto Force = GetForwardVector() * Throttle * MaxDrivingForce;
	auto ForceLocation = GetComponentLocation();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(Force, ForceLocation);

	auto Velocity = (GetOwner()->GetVelocity()) * 0.036;
	UE_LOG(LogTemp, Warning, TEXT("Velocity : %s km/h"), *(Velocity.ToString()))

}

