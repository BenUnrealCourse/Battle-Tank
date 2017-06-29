// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"


UTankTrack::UTankTrack()
{

	PrimaryComponentTick.bCanEverTick = true;
}

void UTankTrack::BeginPlay()
{
	TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent()); // Cast to access mass and calculate force
}

void UTankTrack::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	if (!ensure(TankRoot)) { return; }
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
	auto CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();
	
	auto CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2; //Two Tracks
	TankRoot->AddForce(CorrectionForce);
}

void UTankTrack::Accelerate(float Throttle)
{
	Throttle = FMath::Clamp<float>(Throttle, -1, 1);
	auto Force = GetForwardVector() * Throttle * MaxDrivingForce;
	auto ForceLocation = GetComponentLocation();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(Force, ForceLocation);

	auto Velocity = (GetOwner()->GetVelocity()) * 0.036;

}

