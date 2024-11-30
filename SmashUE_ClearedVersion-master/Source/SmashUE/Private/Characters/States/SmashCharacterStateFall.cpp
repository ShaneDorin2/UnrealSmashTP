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

	CurrentFallDuration = 0;

	Character->InputJumpEvent.AddDynamic(this, &USmashCharacterStateFall::OnInputJump);
	
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

	Character->InputJumpEvent.RemoveDynamic(this, &USmashCharacterStateFall::OnInputJump);
	
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

	CurrentFallDuration += DeltaTime;
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		0.1f,
		FColor::Green,
		TEXT("Tick StateFall")
	);
	
	Character->AddMovementInput(FVector(1, 0, 0), Character->GetInputMoveX());
	
	if (Character->GetInputMoveY() < 0)
	{
		Character->GetCharacterMovement()->GravityScale = FallFastGravityScale;
	}

	if (Character->GetMovementComponent()->Velocity.Z == 0)
	{
		if (CurrentFallDuration >= MinFallDurationToLeadToPRONEState)
		{
			StateMachine->ChangeState(ESmashCharacterStateID::OnGround);
			return;
		}
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
}

void USmashCharacterStateFall::OnInputJump()
{
	StateMachine->ChangeState(ESmashCharacterStateID::Jump);
}



