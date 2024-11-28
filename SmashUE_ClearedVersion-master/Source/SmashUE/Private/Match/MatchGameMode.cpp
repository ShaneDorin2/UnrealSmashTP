// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/MatchGameMode.h"

#include "SourceControlOperations.h"
#include "Arena/AreaSettings.h"
#include "Arena/ArenaPlayerStart.h"
#include "Characters/SmashCharacter.h"
#include "Characters/SmashCharacterSettings.h"
#include "Kismet/GameplayStatics.h"
#include "InputMappingContext.h"
#include "LocalMultiplayerSubsystem.h"
#include "Characters/SmashCharacterInputData.h"

void AMatchGameMode::FindPlayerStartActionInArena(TArray<AArenaPlayerStart*>& ResultsActors)
{
	TArray<AActor*> FoundActors;
	
	// finds all actors of class AArenaPlayerStart in the current 'world' and puts them in the FoundActors array
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArenaPlayerStart::StaticClass(), FoundActors);

	for (int i = 0; i < FoundActors.Num(); i++)
	{
		// cast each actor as a arenaPlayerStart actor. 
		AArenaPlayerStart* ArenaPlayerStartActor = Cast<AArenaPlayerStart>(FoundActors[i]);
		if (ArenaPlayerStartActor == nullptr) continue;

		// add to the reference of the list of arenaPlayerStart pointers
		ResultsActors.Add(ArenaPlayerStartActor);
	}
}

void AMatchGameMode::BeginPlay() // overriding BeginPlay
{
	Super::BeginPlay(); // includes everything in the original BeginPlay
	CreateAndInitPlayers();
	
	TArray<AArenaPlayerStart*> PlayerStartsPoints;
	FindPlayerStartActionInArena(PlayerStartsPoints); // the [&] indicates that it is taking in a REFERENCE (therefor does not need to return)
	SpawnCharacters(PlayerStartsPoints);
	
	for (AArenaPlayerStart* PlayerStartPoint : PlayerStartsPoints) //forEach
	{

		EAutoReceiveInput::Type InputType = PlayerStartPoint->AutoReceiveInput.GetValue();
		TSubclassOf<ASmashCharacter> SmashCharacterClass = GetSmashCharacterClassFromInputType(InputType);
		if (SmashCharacterClass == nullptr) continue;

		// prints message on screen
		GEngine->AddOnScreenDebugMessage( 
		-1,
		3.f,
		FColor::Cyan,
		SmashCharacterClass->GetFName().ToString()
		);
	}
}

TSubclassOf<ASmashCharacter> AMatchGameMode::GetSmashCharacterClassFromInputType(EAutoReceiveInput::Type InputType) const
{
	const UAreaSettings* ArenaSettings = GetDefault<UAreaSettings>();
	
	// based on the input type, return a SmashPlayerSub-class (pulled from the UPROPERTY inputs in the details window).
	switch (InputType)
	{
	case EAutoReceiveInput::Player0:
		return ArenaSettings->SmashCharacterClassP0;
		
	case EAutoReceiveInput::Player1:
		return ArenaSettings->SmashCharacterClassP1;
		
	case EAutoReceiveInput::Player2:
		return ArenaSettings->SmashCharacterClassP2;
		
	case EAutoReceiveInput::Player3:
		return ArenaSettings->SmashCharacterClassP3;
		
	default:
		return nullptr;
	}
}

void AMatchGameMode::CreateAndInitPlayers() const
{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance == nullptr) return;

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (LocalMultiplayerSubsystem == nullptr) return;

	LocalMultiplayerSubsystem->CreateAndInitPlayers(ELocalMultiplayerInputMappingType::InGame);
}

void AMatchGameMode::SpawnCharacters(const TArray<AArenaPlayerStart*>& SpawnPoints)
{

	USmashCharacterInputData* InputData = LoadInputDataFromConfig();
	UInputMappingContext* InputMappingContext = LoadInputMappingContextFromConfig();
	
	for (AArenaPlayerStart* SpawnPoint : SpawnPoints)
	{
		EAutoReceiveInput::Type InputType = SpawnPoint->AutoReceiveInput.GetValue();
		TSubclassOf<ASmashCharacter> SmashCharacterClass = GetSmashCharacterClassFromInputType(InputType);
		if (SmashCharacterClass == nullptr) continue;

		ASmashCharacter* NewCharacter = GetWorld()->SpawnActorDeferred<ASmashCharacter>(
			SmashCharacterClass,
			SpawnPoint->GetTransform()
		);

		if (NewCharacter == nullptr) continue;

		NewCharacter -> InputData = InputData;
		NewCharacter -> InputMappingContext = InputMappingContext;
		
		NewCharacter -> AutoPossessPlayer = SpawnPoint->AutoReceiveInput;
		NewCharacter -> SetOrientX(SpawnPoint -> GetStartOrientX());
		NewCharacter -> FinishSpawning(SpawnPoint->GetTransform());

		CharactersInsideArena.Add(NewCharacter);
	}
}

USmashCharacterInputData* AMatchGameMode::LoadInputDataFromConfig()
{
	const USmashCharacterSettings* CharacterSettings = GetDefault<USmashCharacterSettings>();
	if (CharacterSettings == nullptr) return nullptr;
	return CharacterSettings->InputData.LoadSynchronous();
}

UInputMappingContext* AMatchGameMode::LoadInputMappingContextFromConfig()
{
	const USmashCharacterSettings* CharacterSettings = GetDefault<USmashCharacterSettings>();
	if (CharacterSettings == nullptr) return nullptr;
	return CharacterSettings->InputMappingContext.LoadSynchronous();
}


