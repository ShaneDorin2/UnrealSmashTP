// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerGameViewportClient.h"

#include "LocalMultiplayerSettings.h"
#include "LocalMultiplayerSubsystem.h"

void ULocalMultiplayerGameViewportClient::PostInitProperties()
{
	Super::PostInitProperties();

	MaxSplitscreenPlayers = 8;
}

bool ULocalMultiplayerGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	Super::InputKey(EventArgs);
	
	const ULocalMultiplayerSettings* LocalMultiplayerSettings = GetDefault<ULocalMultiplayerSettings>();
	int KeyboardProfileIndex = LocalMultiplayerSettings->FindKeyboardProfileIndexFromKey(EventArgs.Key, ELocalMultiplayerInputMappingType::Menu);
	const ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GetDefault<ULocalMultiplayerSubsystem>();
	LocalMultiplayerSubsystem->GetAssignedPlayer

}

bool ULocalMultiplayerGameViewportClient::InputAxis(FViewport* Viewport, FInputDeviceId InputDevice, FKey Key,
	float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	return Super::InputAxis(Viewport, InputDevice, Key, Delta, DeltaTime, NumSamples, bGamepad);
}
