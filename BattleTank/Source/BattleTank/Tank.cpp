// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "Projectile.h"
#include "Engine/World.h"
#include "TankMovementComponent.h"

// Called when the game starts or when spawned
void ATank::BeginPlay()
{
	Super::BeginPlay();
}

void ATank::AimAt(FVector HitLocation)
{
	if (!TankAimingComponent) { return; }
	TankAimingComponent->AimAt(HitLocation, LaunchSpeed);

}
void ATank::Fire()
{
	bool isReloaded = (GetWorld()->GetTimeSeconds() - LastTimeFire) > TimeToReloadInSeconds;
	if (Barrel && isReloaded)
	{
		//Spawn Projectile
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetForwardVector().Rotation()
			);
		Projectile->LaunchProjectile(LaunchSpeed);
		LastTimeFire = GetWorld()->GetTimeSeconds();

	}
	else
	{
	}
}

