// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"

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
	GetSightRayHitLocation(HitLocation);
}

bool ATankPlayerController::GetSightRayHitLocation(FVector & HitLocation) const
{
	//Find Crosshair position in pixel coordinates
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	//Get Position in pixels the crosshair is at screen
	auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);
	return GetLookDirection(ViewportSizeX, ViewportSizeY, ScreenLocation);
}

bool ATankPlayerController::GetLookDirection(const int32 &ViewportSizeX, const int32 &ViewportSizeY, FVector2D &ScreenLocation) const
{
	FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);
	//UE_LOG(LogTemp, Warning, TEXT("Crosshair screen´s position : %s"), *ScreenLocation.ToString())
	//"De-Project"screen position of crosshair to game world
	FVector WorldLocation;
	FVector WorldDirection;
	bool bDeprojectionSuccesful = DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		WorldLocation,
		WorldDirection
	);
	//Log out world direction; Ignore World Location (Camera location)
	//UE_LOG(LogTemp, Warning, TEXT("Crosshair Direction: %s"), *WorldDirection.ToString())
	return bDeprojectionSuccesful;
}


