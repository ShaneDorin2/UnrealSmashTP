// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/SmashCharacterStateOnGround.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Characters/SmashCharacterStateMachine.h"

ESmashCharacterStateID USmashCharacterStateOnGround::GetStateID()
{
	return ESmashCharacterStateID::OnGround;
}

void USmashCharacterStateOnGround::StateEnter(ESmashCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);

	Character->PlayAnimMontage(FallDownMontage);

	CurrentProneDuration = 0;
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Cyan,
		TEXT("Enter StatePRONE")
	);
}

void USmashCharacterStateOnGround::StateExit(ESmashCharacterStateID NextStateID)
{
	Super::StateExit(NextStateID);

	GEngine->AddOnScreenDebugMessage(
		-1,
		3.f,
		FColor::Red,
		TEXT("Exit StatePRONE")
	);
}

void USmashCharacterStateOnGround::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);

	CurrentProneDuration += DeltaTime;

	if (CurrentProneDuration > MaxProneDuration - GettingUp->CalculateSequenceLength() &&
		Character->GetCurrentMontage() != GettingUp)
	{
		Character->PlayAnimMontage(GettingUp);
	}

	if (CurrentProneDuration > MaxProneDuration)
	{
		StateMachine->ChangeState(ESmashCharacterStateID::Idle);
	}
	
	GEngine->AddOnScreenDebugMessage(
		-1,
		0.1f,
		FColor::Green,
		TEXT("Tick StatePRONE")
	);
}


