// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Tank.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTankDelegate);

UCLASS()
class BATTLETANK_API ATank : public APawn
{
	GENERATED_BODY()

public:
	// Returns current health as a percentage of starting health, between 0 and 1
	UFUNCTION(BlueprintPure, Category= Health) //BlueprintPure means it cannot modify the owning bp class
	float GetHealthPercentage() const;

	FTankDelegate OnDeath;
protected:
	
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	int32 StartingHealth = 100;

	UPROPERTY(VisibleAnywhere, Category = Health)
	int32 CurrentHealth ;
private:
	ATank();
	void BeginPlay() override;

	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

};
