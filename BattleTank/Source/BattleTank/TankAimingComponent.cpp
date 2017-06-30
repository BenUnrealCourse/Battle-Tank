// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::Initialise(UTankBarrel * BarrelToSet, UTankTurret * TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

// Called when the game starts
void UTankAimingComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bool isReloaded = (GetWorld()->GetTimeSeconds() - LastTimeFire) < TimeToReloadInSeconds;
	if (isReloaded)
	{
		FiringState = EFiringState::Reloading;
	}
	else if (IsBarrelMoving())
	{
		FiringState = EFiringState::Aiming;
	}
	else if (IsOutOfAmmo())
	{
		FiringState = EFiringState::OutOfAmmo;
	}
	else
	{
		FiringState = EFiringState::Locked;
	}
	
}

bool UTankAimingComponent::IsBarrelMoving()
{
	auto BarrelCurrentDirection = Barrel->GetForwardVector().GetSafeNormal();
	return !BarrelCurrentDirection.Equals(AimDirection, 0.01);
}

bool UTankAimingComponent::IsOutOfAmmo()
{
	if (GetRemainingAmmo() == 0) { return true; }
	return false;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!Barrel || !Turret ) { return; }
	FVector OutLaunchVelocity;
	auto StartLocation = Barrel->GetSocketLocation(FName("Projectile")); //It returns barrel location in case of not finding socket name
	//Calculate the velocity
	bool bCalculationSucceded = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutLaunchVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);
	if (bCalculationSucceded)
	{
		AimDirection = OutLaunchVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
		auto Time = GetWorld()->GetTimeSeconds();

	}
	
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	if (!ensure(Barrel)) { return; }
	//Get Difference between barrel rotation and aim rotation
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimDirectionRotator = AimDirection.Rotation();
	auto DeltaRotator = AimDirectionRotator - BarrelRotator;
	//Move the barrel this amount this frame
	//Considering frame time and elevation speed
	Barrel->Elevate(DeltaRotator.Pitch);
	if (FMath::Abs(DeltaRotator.Yaw) < 180)
	{
		Turret->Rotate(DeltaRotator.Yaw);	
	}
	else
	{
		Turret->Rotate(-DeltaRotator.Yaw);
	}
}

void UTankAimingComponent::Fire()
{
	if (!ensure(Barrel)) { return; }
	if (!ensure(ProjectileBlueprint)) { return; }
	
	if (FiringState == EFiringState::Locked)
	{
		if (Shots < MaxAmmo)
		{
			//Spawn Projectile
			auto Projectile = GetWorld()->SpawnActor<AProjectile>(
				ProjectileBlueprint,
				Barrel->GetSocketLocation(FName("Projectile")),
				Barrel->GetForwardVector().Rotation()
				);
			Projectile->LaunchProjectile(LaunchSpeed);
			LastTimeFire = GetWorld()->GetTimeSeconds();
			FiringState = EFiringState::Reloading;

			Shots = Shots + 1;
			UE_LOG(LogTemp, Warning, TEXT("Shots: %i"), Shots)
		}
		}

}

EFiringState UTankAimingComponent::GetFiringState() const
{
	return FiringState;
}

int32 UTankAimingComponent::GetRemainingAmmo() const
{
	return MaxAmmo - Shots;
}
