// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SmashCharacterState.generated.h"
class USmashCharacterStateMachine;
class ESmashCharacterStateID; // I had to add this
class ASmashCharacter;
class ASmashCharacterStateMachine;

UCLASS(ABSTRACT)
class SMASHUE_API USmashCharacterState : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USmashCharacterState();

	virtual ESmashCharacterStateID GetStateID(); // why text red ?

	virtual void StateInit(USmashCharacterStateMachine* InStateMachine);

protected:
	UPROPERTY()
	TObjectPtr<ASmashCharacter> Character;

	UPROPERTY()
	TObjectPtr<USmashCharacterStateMachine> StateMachine;
}; 
