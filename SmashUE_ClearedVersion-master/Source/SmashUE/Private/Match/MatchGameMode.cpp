// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/MatchGameMode.h"
#include "Arena/ArenaPlayerStart.h"
#include "Kismet/GameplayStatics.h"

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

	TArray<AArenaPlayerStart*> PlayerStartsPoints;
	FindPlayerStartActionInArena(PlayerStartsPoints); // the [&] indicates that it is taking in a REFERENCE (therefor does not need to return)

	for (AArenaPlayerStart* PlayerStartPoint : PlayerStartsPoints) //forEach
	{
		// prints message on screen
		GEngine->AddOnScreenDebugMessage( 
		-1,
		3.f,
		FColor::Cyan,
		PlayerStartPoint->GetFName().ToString()
		);
	}
}

