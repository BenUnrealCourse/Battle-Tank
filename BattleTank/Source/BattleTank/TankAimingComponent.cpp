// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TankBarrel.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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

	// ...
}

void UTankAimingComponent::AimAt(FVector HitLocation,float TossSpeed)
{
	if (!Barrel) { return; }
	FVector OutTossVelocity;
	auto StartLocation = Barrel->GetSocketLocation(FName("Projectile")); //It returns barrel location in case of not finding socket name
	//Calculate the velocity
	bool bCalculationSucceded = UGameplayStatics::SuggestProjectileVelocity(
		this,
		OutTossVelocity,
		StartLocation,
		HitLocation,
		TossSpeed,
		false,
		0,
		0,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);
	if (bCalculationSucceded)
	{
		auto AimDirection = OutTossVelocity.GetSafeNormal();
		MoveBarrelTowards(AimDirection);
		auto Time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("%f:Aim Solution Found"))
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Aim calculation not succeded"))
	}
	
}

void UTankAimingComponent::MoveBarrelTowards(FVector AimDirection)
{
	//Get Difference between barrel rotation and aim rotation
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimDirectionRotator = AimDirection.Rotation();
	auto DeltaRotator = AimDirectionRotator - BarrelRotator;
	//Move the barrel this amount this frame
	//Considering frame time and elevation speed
	Barrel->Elevate(DeltaRotator.Pitch);
}
void UTankAimingComponent::SetBarrelReference(UTankBarrel * BarrelToSet)
{
	Barrel = BarrelToSet;
}

