// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"

#include "TankAimingComponent.h"
#include "Tank.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay(); //Needed for BeginPlay of BP to run
	AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (AimingComponent)
	{
		AimingComponentFound(AimingComponent);
	}
}

//Called by engine when setting pawn
void ATankPlayerController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		//Register for possessed tank delegate
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnPossessedTankDeath);
	}
}

void ATankPlayerController::Tick(float DeltaTime)
{
	AimTowardsCrosshair();
}

void ATankPlayerController::AimTowardsCrosshair()
{
	FVector OutHitLocation;
	if (DidLineTraceHitSomething(OutHitLocation))
	{	
		if (!ensure(GetPawn())) { return; }
		AimingComponent->AimAt(OutHitLocation);
	}
}

bool ATankPlayerController::DidLineTraceHitSomething(FVector& OutHitLocation) const
{
	FVector LookDirection;
	GetCrosshairDirection(LookDirection);

	//Line Trace along look direction until line trace reach max
	FHitResult OutHit;
	FVector StartLocation = PlayerCameraManager->GetCameraLocation();
	FVector EndLocation = StartLocation + LookDirection * LineTraceMaxReach;
	bool bLineTraceSuccess = GetWorld()->LineTraceSingleByChannel(
		OutHit,
		StartLocation,
		EndLocation,
		ECollisionChannel::ECC_Camera
	);
	OutHitLocation = OutHit.Location;
	return bLineTraceSuccess;
}

void ATankPlayerController::GetCrosshairDirection(FVector& OutLookDirection) const
{
	//Get Position of Crosshair in Screen Coordinates
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	auto ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);

	//"De-Project"screen position of crosshair to game world
	FVector WorldLocation; //It only serves as output for below function. It´s Camera´s position.
	bool bDeprojectionSuccesful = DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		WorldLocation,
		OutLookDirection
	);
}

void ATankPlayerController::OnPossessedTankDeath()
{
	StartSpectatingOnly();
}




