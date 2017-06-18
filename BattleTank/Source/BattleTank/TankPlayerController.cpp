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
ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}

