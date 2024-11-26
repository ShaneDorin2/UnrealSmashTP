// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "LocalMultiplayerSettings.h"

void ULocalMultiplayerSubsystem::CreateAndInitPlayer(ELocalMultiplayerInputMappingType MappingType)
{
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();

	for (int i =0; i < LocalMultiplayerSettings->GetNbKeyboardProfiles(); i++)
	{
		UGameplayStatics::CreatePlayer(this->GetWorld(), LastAssignedPlayerIndex, true);
		LastAssignedPlayerIndex++;
	}
	LastAssignedPlayerIndex=0;
	for (int i=0; i < LocalMultiplayerSettings->NbMaxGamepads; i++)
	{
		UGameplayStatics::CreatePlayer(this->GetWorld(), LastAssignedPlayerIndex, true);
		LastAssignedPlayerIndex++;
	}
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex)
{
	if (KeyboardProfileIndex > LastAssignedPlayerIndex) return -1;
	return PlayerIndexFromKeyboardProfileIndex[KeyboardProfileIndex];
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex)
{
	LastAssignedPlayerIndex++;
	PlayerIndexFromKeyboardProfileIndex[KeyboardProfileIndex] = LastAssignedPlayerIndex;
	return LastAssignedPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex, ELocalMultiplayerInputMappingType MappingType) const
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this->GetWorld(), PlayerIndex);
	PlayerController->SetInputMappingContext(KeyboardProfileIndex, MappingType);
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID)
{
	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadDeviceID(int DeviceID)
{
	return -1;
}

void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex,
	ELocalMultiplayerInputMappingType MappingType) const
{
}
