// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateRun.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterStateMachine.h"

ESmashCharacterStateID USmashCharacterStateRun::GetStateID()
{
	return ESmashCharacterStateID::Run;
}

void USmashCharacterStateRun::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->PlayAnimMontage(RunMontage);

	Character->InputJumpEvent.AddDynamic(this, &USmashCharacterStateRun::OnInputJump);
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter StateRun")
	);
}

void USmashCharacterStateRun::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	Character->InputJumpEvent.RemoveDynamic(this, &USmashCharacterStateRun::OnInputJump);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Exit StateRun")
	);
}

void USmashCharacterStateRun::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	if (FMath::Abs(Character->GetInputMoveX()) < InputMoveXThreshold)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	else
	{
		Character->SetOrientX(Character->GetInputMoveX());
		Character->AddMovementInput(FVector::ForwardVector, Character->GetOrientX());
	}

	GEngine->AddOnScreenDebugMessage(
		-1,
		0.1f,
		FColor::Green,
		TEXT("Tick StateRun")
	);
}

void USmashCharacterStateRun::OnInputJump()
{
	StateMachine->ChangeState(ESmashCharacterStateID::Jump);
}





