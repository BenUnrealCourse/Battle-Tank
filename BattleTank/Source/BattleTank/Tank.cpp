// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"

ATank::ATank()
{
	PrimaryActorTick.bCanEverTick = false;
}

float ATank::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	int32 DamagePoints = FPlatformMath::RoundToInt(Damage); //Just to not compare between floats
	int32 ActualDamage = FMath::Clamp(DamagePoints, 0, CurrentHealth);

	CurrentHealth = CurrentHealth - ActualDamage;
	if (CurrentHealth <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s died!"),*(GetName()))
	}
	return ActualDamage;
}
