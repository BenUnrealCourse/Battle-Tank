// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"

void ATankPlayerController::BeginPlay()
{
	ATank* PossessedTank = GetControlledTank();
	if (PossessedTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is possessed by the player"), *(PossessedTank->GetName()))
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("No tank possessed by the player"))
	}
	
}

void ATankPlayerController::Tick(float DeltaTime)
{
	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
	FVector HitLocation;
	if (GetSightRayHitLocation(HitLocation))
	{
		GetControlledTank()->AimAt(HitLocation);
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
	//UE_LOG(LogTemp, Warning, TEXT("Crosshair screen´s position : %s"), *ScreenLocation.ToString())
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
		UE_LOG(LogTemp,Warning,TEXT("Crosshair Hit Location : %s"),*(OutHit.Location.ToString()))
	}
	return bLineTraceSuccess;
}



