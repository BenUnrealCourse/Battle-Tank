// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"
#include "Engine/World.h"

void UTankTurret::Rotate(float RelativeSpeed)
{
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);
	auto AzimuthChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds; // 0 when Aim´s rotation == turret´s rotation
	auto NewAzimuth = RelativeRotation.Yaw + AzimuthChange;
	auto NewAzimuthClamped = FMath::Clamp<float>(NewAzimuth, MinAzimuthDegree, MaxAzimuthDegree);
	SetRelativeRotation(FRotator(0, NewAzimuthClamped, 0));

}

