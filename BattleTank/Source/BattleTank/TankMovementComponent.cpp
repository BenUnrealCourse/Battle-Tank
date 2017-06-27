// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialise(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	if (!LeftTrackToSet || !RightTrackToSet) { return; }
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;

}
void UTankMovementComponent::IntendMoveForward(float Throw)
{
	LeftTrack->Accelerate(Throw);
	RightTrack->Accelerate(Throw);
	//TODO prevent double speed due to dual control use
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	LeftTrack->Accelerate(Throw);
	RightTrack->Accelerate(-Throw);
	//TODO prevent double speed due to dual control use
}