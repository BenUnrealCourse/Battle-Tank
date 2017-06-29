// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"

#include "TankAimingComponent.h"


void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay(); //Needed for BeginPlay of BP to run
	AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (AimingComponent)
	{
		AimingComponentFound(AimingComponent);
	}
}

void ATankPlayerController::Tick(float DeltaTime)
{
	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	FVector HitLocation;
	if (GetSightRayHitLocation(HitLocation))
	{	
		if (!ensure(GetPawn())) { return; }
		AimingComponent->AimAt(HitLocation);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	//Find Crosshair position in pixel coordinates
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	//Get Position in pixels the crosshair is at screen
	auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);
	FVector LookDirection;
	GetLookDirection(ViewportSizeX, ViewportSizeY, ScreenLocation, LookDirection);

	//Line Trace along look direction until line trace reach max
	FHitResult Hit;
	bool bLineTracingSuccess = GetLookVectorHitLocation(LookDirection, Hit);
	OutHitLocation = Hit.Location;
	return bLineTracingSuccess;
}

bool ATankPlayerController::GetLookDirection(const int32 &ViewportSizeX, const int32 &ViewportSizeY, FVector2D &ScreenLocation, FVector& OutLookDirection) const
{
	FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);
	//"De-Project"screen position of crosshair to game world
	FVector WorldLocation; //To be discarded : Camera´s position
	bool bDeprojectionSuccesful = DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		WorldLocation,
		OutLookDirection
	);
	return bDeprojectionSuccesful;
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector& LookDirection, FHitResult& OutHit) const
{
	FVector StartLocation = PlayerCameraManager->GetCameraLocation();
	FVector EndLocation = StartLocation + LookDirection * LineTraceMaxReach;
	bool bLineTraceSuccess = GetWorld()->LineTraceSingleByChannel(
		OutHit,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility
	);
	if (bLineTraceSuccess)
	{
	}
	return bLineTraceSuccess;
}



