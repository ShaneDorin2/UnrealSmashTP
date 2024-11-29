// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"

#include "EnhancedInputSubsystemInterface.h"
#include "Kismet/GameplayStatics.h"
#include "LocalMultiplayerSettings.h"
#include "EnhancedInputSubsystems.h"

void ULocalMultiplayerSubsystem::CreateAndInitPlayers(ELocalMultiplayerInputMappingType MappingType)
{
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();

	for (int i =0; i < LocalMultiplayerSettings->GetNbKeyboardProfiles(); i++)
	{
		UGameplayStatics::CreatePlayer(this->GetWorld(), i, true);
	}
	for (int i =0; i < LocalMultiplayerSettings->NbMaxGamepads; i++)
	{
		UGameplayStatics::CreatePlayer(this->GetWorld(), i+4, true);
	}
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromKeyboardProfileIndex(int KeyboardProfileIndex)
{
	if (PlayerIndexFromKeyboardProfileIndex.Contains(KeyboardProfileIndex)) return PlayerIndexFromKeyboardProfileIndex[KeyboardProfileIndex];
	return -1;
	
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToKeyboardProfile(int KeyboardProfileIndex)
{
	LastAssignedPlayerIndex++;
	//PlayerIndexFromKeyboardProfileIndex[KeyboardProfileIndex] = LastAssignedPlayerIndex;
 	PlayerIndexFromKeyboardProfileIndex.Add(KeyboardProfileIndex, LastAssignedPlayerIndex);
	return LastAssignedPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignKeyboardMapping(int PlayerIndex, int KeyboardProfileIndex, ELocalMultiplayerInputMappingType MappingType) const
{
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this->GetWorld(), PlayerIndex);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	Subsystem->ClearAllMappings();
	FModifyContextOptions ModifyContextOptions;
	ModifyContextOptions.bForceImmediately = true;
	Subsystem->AddMappingContext(LocalMultiplayerSettings->KeyBoardProfilesData[KeyboardProfileIndex].GetIMCFromType(MappingType),1, ModifyContextOptions);
}

int ULocalMultiplayerSubsystem::GetAssignedPlayerIndexFromGamepadDeviceID(int DeviceID)
{
	if (PlayerIndexFromGamepadProfileIndex.Contains(DeviceID)) return PlayerIndexFromGamepadProfileIndex[DeviceID];
	return -1;
}

int ULocalMultiplayerSubsystem::AssignNewPlayerToGamepadDeviceID(int DeviceID)
{
	LastAssignedPlayerIndex++;
	PlayerIndexFromGamepadProfileIndex[DeviceID] = LastAssignedPlayerIndex;
	return LastAssignedPlayerIndex;
}

void ULocalMultiplayerSubsystem::AssignGamepadInputMapping(int PlayerIndex, ELocalMultiplayerInputMappingType MappingType) const
{
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this->GetWorld(), PlayerIndex);
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	Subsystem->ClearAllMappings();
	FModifyContextOptions ModifyContextOptions;
	ModifyContextOptions.bForceImmediately = true;
	Subsystem->AddMappingContext(LocalMultiplayerSettings->GamepadProfileData.GetIMCFromType(MappingType), 1, ModifyContextOptions);
}
