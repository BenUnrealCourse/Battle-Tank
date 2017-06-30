// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankBarrel.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankBarrel : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	//Negative numbers get barrel down, positive numbers get barrel up
	void Elevate(float RelativeSpeed);

private:
	UPROPERTY(EditAnywhere , Category = Setup)
	float MaxDegreesPerSecond = 10;

	UPROPERTY(EditAnywhere, Category = Setup)
	float MaxElevationDegrees = 45;

	UPROPERTY(EditAnywhere, Category = Setup)
	float MinElevationDegrees = 0;
};
