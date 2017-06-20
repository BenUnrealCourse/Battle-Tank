// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"

void ATankAIController::BeginPlay()
{
	auto ControlledTank = GetControlledTank();
	if (ControlledTank)
	{
		UE_LOG ( LogTemp, Warning , TEXT("%s is possessed by TankAIController"), *(ControlledTank->GetName()))
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("There is a tank not possessed by TankAIController"))
	}
}
void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("TANK AI CONTROLLER TICK"))
	GetControlledTank()->AimAt(GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
}
ATank* ATankAIController::GetControlledTank() const
{
	return Cast<ATank>(GetPawn());
}


