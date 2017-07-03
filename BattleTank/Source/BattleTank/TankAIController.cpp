// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "TankAimingComponent.h"
#include "Tank.h"
//Depends on Movement Component indirectly via pathfinding logic of MoveToActor

ATankAIController::ATankAIController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();

}

//TODO Tick(float) Never being called
void ATankAIController::Tick(float DeltaTime)
{
	if (!ensure(AimingComponent)) { return; }
	Super::Tick(DeltaTime);
	//move 
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	MoveToActor(PlayerTank, AcceptanceRadius);
	AimingComponent->AimAt(GetPlayerTankLocation());
	AimingComponent->Fire();
}

FVector ATankAIController::GetPlayerTankLocation()
{
	return GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
}

void ATankAIController::OnPossessedTankDeath()
{
	UE_LOG(LogTemp,Warning,TEXT("%s died!"), *(GetPawn()->GetName()))
}

void ATankAIController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		//Register for possessed tank delegate
		auto PossessedTank = Cast<ATank>(InPawn);
		if (!ensure(PossessedTank)) { return; }
		PossessedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnPossessedTankDeath);
	}
}
