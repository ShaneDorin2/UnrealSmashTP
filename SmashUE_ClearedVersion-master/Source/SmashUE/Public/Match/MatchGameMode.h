// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MatchGameMode.generated.h"

class AArenaPlayerStart; // this is a forward

/**
 * 
 */
UCLASS()
class SMASHUE_API AMatchGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	// in the .cpp , we will be redefining GameModeBase's BeginPlay function. 
	virtual void BeginPlay() override;
	
private:
	// a function that takes in a REFERENCE of an array of POINTERS of arenaPlayerStart objects
	void FindPlayerStartActionInArena(TArray<AArenaPlayerStart*>& ResultsActors );
};
