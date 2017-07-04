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

//Called by the engine when setting pawn
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

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();

}

//Called by the engine every frame
void ATankAIController::Tick(float DeltaTime)
{
	if (!ensure(AimingComponent)) { return; }
	Super::Tick(DeltaTime);

	AimingComponent->AimAt(GetPlayerTankLocation());

	//move if tank player controller has a pawn attached
	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!ensure(PlayerTank)) { return; }
	MoveToActor(PlayerTank, AcceptanceRadius);

	//TODO Only Fire if sees player´s tank
	FHitResult Hit;
	auto DirectionBetweenPoints = (GetPlayerTankLocation() - AimingComponent->GetProjectileInitialLocation()).Rotation().Vector();
	auto EndTrace = AimingComponent->GetProjectileInitialLocation() + DirectionBetweenPoints * 100000;
	auto bHitSomething = GetWorld()->LineTraceSingleByChannel(
		Hit,
		AimingComponent->GetProjectileInitialLocation(),
		EndTrace,
		ECollisionChannel::ECC_Visibility
	);
	if (bHitSomething)
	{
		if (Cast<ATank>(Hit.GetActor()))
		{
			AimingComponent->Fire();
		}
	}

}

FVector ATankAIController::GetPlayerTankLocation()
{
	if (!GetWorld()->GetFirstPlayerController()->GetPawn()) { return FVector(0);  }
	return GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
}

void ATankAIController::OnPossessedTankDeath()
{
	if (!GetPawn()) { return; }
	GetPawn()->DetachFromControllerPendingDestroy();
}
