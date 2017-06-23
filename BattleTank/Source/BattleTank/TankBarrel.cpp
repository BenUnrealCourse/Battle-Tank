// Fill out your copyright notice in the Description page of Project Settings.

#include "TankBarrel.h"
#include "Engine/World.h"


void UTankBarrel::Elevate(float RelativeSpeed)
{
	RelativeSpeed = FMath::Clamp<float>(RelativeSpeed,-1,1);
	//ElevatonChange = DeltaSeconds , RelativeSpeed, MaxDegreesPerSec
	//deg.frame^-1 = sec.frame^-1,const, deg.sec^-1 
	//deg.frame^-1 = deg.frame^-1
	auto ElevationChange = RelativeSpeed * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;  // 0 when Aim´s rotation == turret´s rotation
	auto NewElevation = RelativeRotation.Pitch + ElevationChange; //deg.Frame^-1
	auto NewElevationClamped = FMath::Clamp<float>(NewElevation, MinElevationDegrees, MaxElevationDegrees);
	SetRelativeRotation(FRotator(NewElevationClamped, 0, 0)); // Rot.Frame^-1
}

