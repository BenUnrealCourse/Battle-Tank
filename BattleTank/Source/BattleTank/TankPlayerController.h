// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

//Forward Declarations

class UTankAimingComponent;
/**
 * 
 */
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
public:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float LineTraceMaxReach = 1000000.f;

	UFUNCTION(BlueprintImplementableEvent, Category = Setup)
	void AimingComponentFound(UTankAimingComponent* AimingComponent); //Event must be called bot must not be implemented

private:
	UPROPERTY(EditAnywhere)
	float CrosshairXLocation = 0.5f;

	UPROPERTY(EditAnywhere)
	float CrosshairYLocation = 0.33333f;

	//Start the tank moving the barrel so that a shot would hit where crosshair intersects world
	void AimTowardsCrosshair();

	//Return an OUT param, true if it hit landscape
	bool GetSightRayHitLocation(FVector& HitLocation) const;

	bool GetLookVectorHitLocation(FVector &LookDirection, FHitResult &Hit) const;

	bool GetLookDirection(const int32 &ViewportSizeX, const int32 &ViewportSizeY, FVector2D &ScreenLocation, FVector& LookDirection) const;

	UTankAimingComponent* AimingComponent = nullptr;
};
