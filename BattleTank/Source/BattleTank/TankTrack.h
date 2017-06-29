// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

/**
 * 
 */
UCLASS(meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void Accelerate(float Throttle);
	
protected:
	UPROPERTY(EditDefaultsOnly)
	float MaxDrivingForce = 19000000;

	UTankTrack();

	void BeginPlay() override;

	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UStaticMeshComponent* TankRoot = nullptr;
};
