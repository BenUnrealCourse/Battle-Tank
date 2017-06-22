// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"
#include "Engine/World.h"

void UTankTurret::MoveTurret(float RelativeSpeed)
{
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed, -1, 1);
	auto AzimuthChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
	auto NewAzimuth = RelativeRotation.Yaw + AzimuthChange;
	auto NewAzimuthClamped = FMath::Clamp<float>(NewAzimuth, MinAzimuthDegree, MaxAzimuthDegree);
	SetRelativeRotation(FRotator(0, NewAzimuthClamped, 0));
	UE_LOG(LogTemp,Warning,TEXT("Azimuth Clamped: %s"), *(RelativeRotation.ToString()))

}

