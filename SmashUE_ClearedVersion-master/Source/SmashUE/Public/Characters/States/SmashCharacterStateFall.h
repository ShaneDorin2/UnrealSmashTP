﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/SmashCharacterState.h"
#include "SmashCharacterStateFall.generated.h"


UCLASS(ClassGroup=(SmashCharacterState), meta=(BlueprintSpawnableComponent))
class SMASHUE_API USmashCharacterStateFall : public USmashCharacterState
{
	GENERATED_BODY()

public:
	virtual ESmashCharacterStateID GetStateID() override;
	
	virtual void StateEnter(ESmashCharacterStateID PreviousStateID) override;

	virtual void StateExit(ESmashCharacterStateID NextStateID) override;

	virtual void StateTick(float DeltaTime) override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FallHorizontalMoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FallAirControl;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FallGravityScale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FallFastGravityScale;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* FallMontage;
};
