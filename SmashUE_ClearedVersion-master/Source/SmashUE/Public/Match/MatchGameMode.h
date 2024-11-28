// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MatchGameMode.generated.h"

class UInputMappingContext;
class USmashCharacterInputData;
class ASmashCharacter;
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
	USmashCharacterInputData* LoadInputDataFromConfig();

	UInputMappingContext* LoadInputMappingContextFromConfig();
	
	// a function that takes in a REFERENCE of an array of POINTERS of arenaPlayerStart objects
	void FindPlayerStartActionInArena(TArray<AArenaPlayerStart*>& ResultsActors );

	void SpawnCharacters(const TArray<AArenaPlayerStart*>& SpawnPoints);

	// a function that returns a reference to a SmashCharacter actor based on an 'input type'
	TSubclassOf<ASmashCharacter> GetSmashCharacterClassFromInputType(EAutoReceiveInput::Type InputType) const;

	void CreateAndInitPlayers() const;

protected:
	//display a list of the Characters inside the arena(?)
	UPROPERTY()
	TArray<ASmashCharacter*> CharactersInsideArena;
	
	// adds SmashCharacter variables that can be changed in the Details window (like inspector in Unity)
	/*UPROPERTY(EditAnywhere)
	TSubclassOf<ASmashCharacter> SmashCharacterClassP0;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASmashCharacter> SmashCharacterClassP1;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASmashCharacter> SmashCharacterClassP2;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ASmashCharacter> SmashCharacterClassP3;
	*/
};
