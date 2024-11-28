// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerGameViewportClient.h"

#include "InputState.h"
#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"
#include "GameFramework/PlayerInput.h"
#include "Kismet/GameplayStatics.h"

void ULocalMultiplayerGameViewportClient::PostInitProperties()
{
	Super::PostInitProperties();

	MaxSplitscreenPlayers = 8;
}

bool ULocalMultiplayerGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = UGameInstance::GetSubsystem<ULocalMultiplayerSubsystem>();
	int PlayerIndex = -1;
	
	if (EventArgs.Key.IsGamepadKey())
	{
		PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadDeviceID(EventArgs.ControllerId);
		if (PlayerIndex == -1)
		{
			PlayerIndex = LocalMultiplayerSubsystem->AssignNewPlayerToGamepadDeviceID(EventArgs.ControllerId);
			LocalMultiplayerSubsystem->AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::InGame);
		}
	}
	else //key is keyboard
	{
		int KeyboardProfileIndex = LocalMultiplayerSettings->FindKeyboardProfileIndexFromKey(EventArgs.Key, ELocalMultiplayerInputMappingType::InGame);
		if (KeyboardProfileIndex == -1) return false;
		
		PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboardProfileIndex);
		if (PlayerIndex == -1)
		{
			PlayerIndex = LocalMultiplayerSubsystem->AssignNewPlayerToKeyboardProfile(KeyboardProfileIndex);
			LocalMultiplayerSubsystem->AssignKeyboardMapping(PlayerIndex, KeyboardProfileIndex, ELocalMultiplayerInputMappingType::InGame);
		}
	}
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this->GetWorld(), PlayerIndex);
	if (PlayerController == nullptr) return false;
	FInputKeyParams InputParams(EventArgs.Key, EventArgs.Event, double(EventArgs.AmountDepressed), EventArgs.Key.IsGamepadKey());
	PlayerController->InputKey(InputParams);
	return true;
}

bool ULocalMultiplayerGameViewportClient::InputAxis(FViewport* Viewport, FInputDeviceId InputDevice, FKey Key,
	float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = UGameInstance::GetSubsystem<ULocalMultiplayerSubsystem>();
	int PlayerIndex = -1;
	
	if (bGamepad)
	{
		PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromGamepadDeviceID(InputDevice.GetId());
		if (PlayerIndex == -1)
		{
			PlayerIndex = LocalMultiplayerSubsystem->AssignNewPlayerToGamepadDeviceID(InputDevice.GetId());
			LocalMultiplayerSubsystem->AssignGamepadInputMapping(PlayerIndex, ELocalMultiplayerInputMappingType::InGame);
		}
	}
	else //input is keyboard
	{
		int KeyboeardProfileIndex = LocalMultiplayerSettings->FindKeyboardProfileIndexFromKey(Key, ELocalMultiplayerInputMappingType::InGame);
		if (KeyboeardProfileIndex == -1) return false;

		PlayerIndex = LocalMultiplayerSubsystem->GetAssignedPlayerIndexFromKeyboardProfileIndex(KeyboeardProfileIndex);
		if (PlayerIndex == -1)
		{
			PlayerIndex = LocalMultiplayerSubsystem->AssignNewPlayerToKeyboardProfile(KeyboeardProfileIndex);
			LocalMultiplayerSubsystem->AssignKeyboardMapping(PlayerIndex, KeyboeardProfileIndex, ELocalMultiplayerInputMappingType::InGame);
		}
	}
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this->GetWorld(), PlayerIndex);
	if (PlayerController == nullptr) return false;
	FInputKeyParams InputParams(Key, double(Delta), DeltaTime, NumSamples, bGamepad);
	PlayerController->InputKey(InputParams);
	return true;
}