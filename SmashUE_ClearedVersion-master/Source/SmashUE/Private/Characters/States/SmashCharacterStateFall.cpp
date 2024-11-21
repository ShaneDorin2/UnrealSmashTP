// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateFall.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateFall::GetStateID()
{
	return ESmashCharacterStateID::Fall;
}

void USmashCharacterStateFall::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->PlayAnimMontage(FallMontage);

	Character->GetCharacterMovement() -> AirControl = FallAirControl;
	Character->GetCharacterMovement() -> MaxWalkSpeed = FallHorizontalMoveSpeed;
  	Character->GetCharacterMovement() -> GravityScale = FallGravityScale;
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter StateFall")
	);
}

void USmashCharacterStateFall::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	Character->GetCharacterMovement()->GravityScale = 1;
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Exit StateFall")
	);
}

void USmashCharacterStateFall::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	GEngine->AddOnScreenDebugMessage(
		-1,
		0.1f,
		FColor::Green,
		TEXT("Tick StateFall")
	);

	Character->SetOrientX(Character->GetInputMoveX());
	Character->AddMovementInput(FVector(1, 0, 0), Character->GetOrientX());
	
	if (Character->GetInputMoveY() < 0)
	{
		Character->GetCharacterMovement()->GravityScale = FallFastGravityScale;
	}

	if (Character->GetMovementComponent()->Velocity.Z == 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
}



