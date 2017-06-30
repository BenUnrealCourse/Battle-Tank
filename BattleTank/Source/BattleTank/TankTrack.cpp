// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "Engine/World.h"


UTankTrack::UTankTrack()
{

	PrimaryComponentTick.bCanEverTick = false;
	OnComponentHit.AddDynamic(this,&UTankTrack::OnHit);
}

void UTankTrack::BeginPlay()
{
	TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent()); // Cast to access mass and calculate force
	
}

void UTankTrack::OnHit(UPrimitiveComponent * HitComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, FVector NormalImpulse, const FHitResult & Hit)
{
	DriveTrack();
	ApplySidewaysForce();
	CurrentThrottle = 0;
}

void UTankTrack::ApplySidewaysForce()
{
	if (!ensure(TankRoot)) { return; }
	auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity()); //  1 * |ComponentVelocity| * Cos@  -> cm/s
	auto DeltaTime = GetWorld()->GetDeltaSeconds();
	auto CorrectionAcceleration = -(SlippageSpeed / DeltaTime) * GetRightVector(); // a = speed / time -> cm/s²
	auto CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2; // F = m * a ...(Each Track will apply same amount of force)
	TankRoot->AddForce(CorrectionForce);
}


void UTankTrack::SetThrottle(float Throttle)
{
	CurrentThrottle = FMath::Clamp<float>(Throttle, -1, 1);

}

void UTankTrack::DriveTrack()
{
	auto ForceIntensity = GetForwardVector() * CurrentThrottle * MaxDrivingForce;
	auto ForceLocation = GetComponentLocation();
	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceIntensity, ForceLocation);

	auto VelocityInKmPerHour = (GetComponentVelocity()) * 0.036;
}

