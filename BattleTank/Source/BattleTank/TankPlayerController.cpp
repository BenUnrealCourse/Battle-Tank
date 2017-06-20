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
	Super::Tick(DeltaTime);
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
	GetControlledTank()->AimAt(HitLocation);
}

bool ATankPlayerController::GetSightRayHitLocation(FVector & HitLocation) const
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
	bool bLineTraceSuccess = GetLookVectorHitLocation(LookDirection, Hit);
	HitLocation = Hit.Location;
	return bLineTraceSuccess;
}

bool ATankPlayerController::GetLookDirection(const int32 &ViewportSizeX, const int32 &ViewportSizeY, FVector2D &ScreenLocation, FVector& LookDirection) const
{
	FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);
	//UE_LOG(LogTemp, Warning, TEXT("Crosshair screen´s position : %s"), *ScreenLocation.ToString())
	//"De-Project" crosshairs´screen position to game world position
	FVector WorldLocation; //To be discarded : Camera´s position
	bool bDeprojectionSuccesful = DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		WorldLocation,
		LookDirection
	);
	return bDeprojectionSuccesful;
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector &LookDirection, FHitResult &Hit) const
{
	FVector StartLocation = PlayerCameraManager->GetCameraLocation();
	FVector EndLocation = StartLocation + LookDirection * LineTraceMaxReach;
	bool bLineTraceSuccess = GetWorld()->LineTraceSingleByChannel(
		Hit,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Visibility
	);
	if (bLineTraceSuccess)
	{
		//UE_LOG(LogTemp,Warning,TEXT("Crosshair Hit Location : %s"),*(Hit.Location.ToString()))
	}
	return bLineTraceSuccess;
}


