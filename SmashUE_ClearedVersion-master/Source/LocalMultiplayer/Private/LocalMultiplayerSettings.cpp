// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSettings.h"
#include "EnhancedInputSubsystemInterface.h"
#include "InputMappingContext.h"


bool FLocalMultiplayerProfileData::ContainsKey(const FKey& Key, ELocalMultiplayerInputMappingType MappingType) const
{
	for (const FEnhancedActionKeyMapping& Mapping : GetIMCFromType(MappingType)->GetMappings())
	{
		if (Mapping.Key == Key) return true;
	}
	return false;
}

UInputMappingContext* FLocalMultiplayerProfileData::GetIMCFromType(ELocalMultiplayerInputMappingType MappingType) const
{
	switch (MappingType)
	{
	case ELocalMultiplayerInputMappingType::InGame:
		return IMCInGame;
	case ELocalMultiplayerInputMappingType::Menu:
		return IMCMenu;
	}
	return nullptr;
}

int ULocalMultiplayerSettings::GetNbKeyboardProfiles() const
{
	return sizeof(KeyBoardProfilesData);
}

int ULocalMultiplayerSettings::FindKeyboardProfileIndexFromKey(const FKey& Key,
	ELocalMultiplayerInputMappingType MappingType) const
{
	for (int i = 0; i < GetNbKeyboardProfiles(); i++)
	{
		if (KeyBoardProfilesData[i].ContainsKey(Key, MappingType))
		{
			return i;
		}
	}
	return -1;
}
