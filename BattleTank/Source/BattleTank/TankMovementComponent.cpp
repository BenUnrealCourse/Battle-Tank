// Fill out your copyright notice in the Description page of Project Settings.

#include "TankMovementComponent.h"
#include "TankTrack.h"

void UTankMovementComponent::Initialise(UTankTrack* LeftTrackToSet, UTankTrack* RightTrackToSet)
{
	
	LeftTrack = LeftTrackToSet;
	RightTrack = RightTrackToSet;

}

void UTankMovementComponent::RequestDirectMove(const FVector & MoveVelocity, bool bForceMaxSpeed)
{
	auto TankForwardDirection = GetOwner()->GetActorForwardVector().GetSafeNormal();
	auto AIForwardIntentionDirection = MoveVelocity.GetSafeNormal();
	
	auto ForwardThrow = FVector::DotProduct(TankForwardDirection, AIForwardIntentionDirection); // 1 * 1 * Cos@ -> { 0 when 90 and 270. 1 when 0 and 180}
	auto RightThrow = FVector::CrossProduct(TankForwardDirection, AIForwardIntentionDirection).Z;// 1 * 1 * Sen@ -> { 0 when 0 and 180. 1 when 90 and 270}

	IntendTurnRight(RightThrow);
	IntendMoveForward(ForwardThrow);
}

void UTankMovementComponent::IntendTurnRight(float Throw)
{
	if (!ensure(LeftTrack && RightTrack)) { return; }
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(-Throw);
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	if (!ensure(LeftTrack && RightTrack)) { return; }
	LeftTrack->SetThrottle(Throw);
	RightTrack->SetThrottle(Throw);
}
