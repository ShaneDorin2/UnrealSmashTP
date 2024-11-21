// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateJump.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateMachine.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"

ESmashCharacterStateID USmashCharacterStateJump::GetStateID()
{
	return ESmashCharacterStateID::Jump;
}

void USmashCharacterStateJump::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->PlayAnimMontage(JumpMontage);

	Character->GetMovementComponent()->Velocity.Normalize();
	Character-> GetCharacterMovement() -> JumpZVelocity = (2*JumpMaxHeight/JumpDuration);
	Character->GetCharacterMovement() -> AirControl = JumpAirControl;
	Character->GetCharacterMovement() -> MaxWalkSpeed = JumpWalkSpeed;
	Character->Jump();

	//Character->InputJumpEvent.AddDynamic(this, &USmashCharacterStateJump::OnInputJump);
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter StateJump")
	);
}

void USmashCharacterStateJump::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Exit StateJump")
	);
}

void USmashCharacterStateJump::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		0.1f,
		FColor::Green,
		TEXT("Tick StateJump")
	);
  	Character->AddMovementInput(FVector(1, 0, 0),  Character->GetInputMoveX());

	if (Character->GetMovementComponent()->Velocity.Z < 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Fall);
	}

	if (Character->GetMovementComponent()->Velocity.Z == 0)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
}



